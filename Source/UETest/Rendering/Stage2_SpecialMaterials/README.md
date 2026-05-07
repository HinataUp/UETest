# Stage 2 - 特殊材质类型（T05-T07 🔀 编辑器为主 + 少量 C++）

## 模块总览
跳出 Opaque / Translucent，看 UE 的"特殊 Material Domain"——它们在渲染管线的不同位置介入。

| Task | 主题 | 文件 |
| --- | --- | --- |
| T05 | Post-Process Material（屏幕空间灰度+暗角）| `Task05_PostProcess/` |
| T06 | Decal Material（弹痕系统）| `Task06_Decal/` |
| T07 | Custom Depth/Stencil（X-Ray 透视描边）| `Task07_CustomStencil/` |

完成后能回答：
- Post-Process 材质的输入是什么？为什么不是 mesh UV？
- Deferred Decal 怎么影响 GBuffer？DBuffer 模式有何不同？
- Custom Depth 是额外一个 pass，性能成本在哪？

## T05 - Post-Process Material：全屏灰度 + 暗角

**步骤：**
1. New Material → Material Domain: **Post Process**，Blend Mode: **Post Process**
2. 用 `SceneTexture:PostProcessInput0` 节点采样当前帧画面
3. 灰度：`Dot(SceneColor.rgb, float3(0.299, 0.587, 0.114))`
4. 暗角：`SceneTexCoord` → 距离中心 → smoothstep 衰减
5. Strength 暴露为 Scalar Parameter，挂到 PostProcessVolume → Post Process Materials 数组

**关键认知：**
- Post-Process 材质运行在**整个场景渲染完成后**，输入是屏幕空间纹理
- `SceneTexture` 可采样 GBuffer 任意通道（WorldNormal、Depth、CustomStencil ...）—— 描边、SSAO 自定义、热成像基础
- 创建 PP 材质 = 编辑器；动态开关 PP 材质 = BP / C++（如夜视模式切换）

## T06 - Deferred Decal：弹痕系统

**编辑器侧：**
1. New Material → Material Domain: **Deferred Decal**，Blend Mode: **Translucent**
2. 弹痕 Texture → Alpha 通道接 Opacity，RGB 接 BaseColor
3. Normal Map 接 Normal（凹陷感）
4. Project Settings → Rendering → 启用 **DBuffer Decals**（让 Decal 影响 BaseColor/Normal/Roughness 而不只是 Emissive）

**C++ 侧（本 task 提供 `T06_DecalSpawner`）：**
```cpp
UT06_DecalSpawner* Spawner = ...;
Spawner->BulletDecalMaterial = MyDecalMI;
Spawner->SpawnBulletHole(HitLocation, HitNormal);
// 5 秒后自动淡出销毁
```

**关键认知：**
- Decal 在 GBuffer 阶段之后、Lighting 之前注入，直接修改 GBuffer 像素
- DBuffer 模式：Decal 在 GBuffer 写入之前介入 → 正确影响光照
- 非 DBuffer：只能影响 Emissive/Opacity
- **Decal Component 的 -X 轴是投影方向**；Spawner 中的 `(-Normal).Rotation()` 让 -X 朝 Normal 反向

## T07 - Custom Depth/Stencil：X-Ray 描边

**步骤：**
1. Project Settings → Rendering → **Custom Depth-Stencil Pass: Enabled with Stencil**
2. 选目标 Mesh → Details → Render Custom Depth=true，Custom Depth Stencil Value=1（或代码 `T07_CustomStencilHelper::EnableXRayOutline`）
3. 创建 Post-Process Material：
   - 采样 `SceneTexture:CustomStencil`，检测 stencil == 1
   - 对邻域做 Sobel 边缘检测 → 输出描边颜色
4. 挂到 PostProcessVolume

**C++ helper：**
```cpp
UT07_CustomStencilHelper::EnableXRayOutline(MeshComp, /*StencilValue=*/1);
// 不想描边时
UT07_CustomStencilHelper::DisableXRayOutline(MeshComp);
```

**关键认知：**
- Custom Depth 是**额外一个 pass**：标记物体多渲染一次到独立深度缓冲
- Stencil 8bit (0-255)，可给不同类别物体编号
- **被墙挡住仍能描边**：因为 PP 材质用 `CustomDepth` 比 `SceneDepth`，绕开正常遮挡判定
- 性能成本：每个标记 mesh 一次额外 draw call —— 大量物体慎用

## 测试方法

**手动 PIE：**
- T05：Strength=0 看正常画面；Strength=1 看到完全灰度+暗角
- T06：开 DBuffer 前后对比，弹痕的 Normal 是否影响光照
- T07：把目标走到墙后，描边仍然显示

**自动化测试：** `UETest.Rendering.渲染模块`
```
Stage 2 - T06 弹痕 Decal Spawner
  - 默认 LifeSpan / FadeDuration 合理
```

## 常见坑
- T05：忘了在 PostProcessVolume 里勾 **Unbound** 或玩家不在 Volume 内 → 看不到效果
- T05：Material Domain 要 PostProcess，**Blend Mode 也得是 PostProcess**
- T06：Decal Component 投影方向是 -X 不是 +Z —— 写错 Rotation 弹痕方向反
- T06：不开 DBuffer 时 Normal 不影响光照 → 弹痕看起来扁平
- T07：Custom Depth Pass 默认关闭 —— 改 Project Settings 后要重启编辑器
