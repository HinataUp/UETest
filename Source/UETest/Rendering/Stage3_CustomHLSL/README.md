# Stage 3 - Custom HLSL（T08-T10 🔀 Material Editor 内写代码）

## 模块总览
跳出节点图，在 Material Editor 中嵌入 HLSL 文本——不需要 C++ 项目代码，但已经在写 shader 代码。

| Task | 主题 |
| --- | --- |
| T08 | Custom Expression 节点（手写 Dissolve）|
| T09 | World Position Offset（旗帜飘动 / Vertex Shader）|
| T10 | Pixel Depth Offset + Parallax Occlusion Mapping |

完成后能回答：
- Custom Node 的 HLSL 是 inline 还是函数？
- WPO 在哪个 shader 阶段？为什么不影响碰撞？
- POM 是真的几何吗？性能瓶颈是什么？

## T08 - Custom Node：Dissolve 溶解

**步骤：**
1. Master Material 中加 `Custom` 节点
2. Inputs：`UV (Vector2)`, `Threshold (Scalar)`, `EdgeWidth (Scalar)`，且需要先建 `Texture Object` 节点接到 NoiseTex 上、把名字写到 Inputs 里
3. Code 栏：
   ```hlsl
   float n = Texture2DSample(NoiseTex, NoiseTexSampler, UV).r;
   float opacity = smoothstep(Threshold - EdgeWidth, Threshold, n);
   float edge   = smoothstep(Threshold, Threshold + EdgeWidth, n) - opacity;
   return float4(opacity, edge, 0, 0);
   ```
4. Output Type: CMOT Float4
5. 主图：`opacity` (R 通道) 接 Opacity Mask（启用 clip）；`edge` (G 通道) → multiply 发光色 → Emissive
6. 调 Threshold 0→1 看完整溶解过程

**关键认知：**
- Custom Node 代码**直接 inline** 到生成的 shader 函数体——不是独立函数
- `Additional Outputs` 可加多个输出 pin
- **局限**：不能定义 struct / 新 function / include；不能访问额外 RenderTarget；不能多 pass
- **MF vs Custom Node**：MF = 节点图复用；Custom Node = HLSL 文本精控

## T09 - WPO：旗帜飘动

**步骤：**
1. 建一个细分的平面 mesh（50×50 face count）
2. 材质 `WorldPositionOffset` 通道接：
   ```
   Vertex 的 ObjectLocalPosition.X * Frequency
     → Add(Time * Speed)
     → Sin
     → Multiply(Amplitude)
     → 输出为 Z 偏移
   ```
3. 同时把偏移大小映射到 BaseColor：静止=蓝、最大摆幅=红

**关键认知：**
- WPO 是材质中**唯一能修改顶点位置**的通道，运行在 **Vertex Shader**
- WPO **只移动渲染位置**，不影响 Collision（Collision 用原始 mesh）
- Nanite mesh 在 5.1+ 支持 WPO（需要 `Evaluate World Position Offset`），但有 Virtual Shadow Map 额外成本
- **WPO vs C++**：周期性 / 程序化变形用 WPO（GPU 端零 CPU 成本）；物理驱动（布料）用 ProceduralMeshComponent 或 Chaos

## T10 - POM + PDO

**步骤（用内置 MF 版）：**
1. 准备 Heightmap（灰度图）
2. 在 Content Browser 找 `ParallaxOcclusionMapping` Material Function（Engine Content）
3. Inputs：HeightmapTexture / MaxSteps（默认 8-16）/ HeightRatio
4. 输出的 UV 用来采样 BaseColor / Normal / Roughness
5. 输出的 PixelDepthOffset 接 `Pixel Depth Offset` 通道

**步骤（手写 Custom Node 版）：**
```hlsl
float2 UV = ScreenUV;
float3 ViewDir = TangentSpaceViewDir;  // 切线空间视角
float Step = 1.0 / MaxSteps;
float CurrHeight = 1.0;
for (int i = 0; i < MaxSteps; ++i) {
    float SampledHeight = Texture2DSample(HeightTex, HeightSampler, UV).r;
    if (CurrHeight <= SampledHeight) break;
    UV += ViewDir.xy * Step * HeightScale;
    CurrHeight -= Step;
}
return float4(UV, CurrHeight, 0);
```

**关键认知：**
- POM 是**纯 Pixel Shader 技术**——不增加几何体，靠 ray marching 偏移采样坐标
- PDO 修改像素写入深度的值 → 让 POM 假凹凸参与正确深度测试 → 其他物体能正确遮挡
- **PDO 不兼容 Nanite，不适用 Translucent**
- 性能：每像素多次纹理采样；掠射角度采样数爆炸 → 必须设 MaxSteps 上限

## 测试方法

**手动 PIE（这阶段必须 PIE）：**
- T08：录一段 Threshold 0→1 动画，观察边缘发光的位置随阈值前进
- T09：摆动效果在 Viewport 看；同时看 Collision 是否纹丝不动（说明 WPO 没改物理）
- T10：相机靠近 POM 表面看凹凸；放小物件在凹坑内看遮挡是否正确

**自动化测试：** 此阶段为 Material Editor 工作，无 Spec 覆盖。

## 常见坑
- T08：Custom Node 的 Inputs 名字必须与 Code 中变量名**完全一致**（含大小写）
- T08：忘记 Output Type → 默认 Float1 输出，多通道丢失
- T09：Mesh 细分不够，旗帜变形看起来割裂
- T09：Nanite mesh 默认不计算 WPO，要在 mesh 属性里勾 `Evaluate World Position Offset`
- T10：MaxSteps 给得太大 → 性能掉到地板；通常 8-16 足够
- T10：POM 用在 Translucent 材质 → 视觉错乱，不要这么做
