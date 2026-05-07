# Stage 1 - Material Editor 基础（T01-T04 🎨 纯编辑器）

## 模块总览
建立"哪些事用编辑器做、哪些事必须写代码"的判断力。本阶段全部用 Material Editor + Material Instance Editor，**零运行时代码**——除了 T04 提供了一个可选的 BP 函数库（运行时驱动 MPC）。

| Task | 主题 | 文件 |
| --- | --- | --- |
| T01 | PBR 材质球展厅（GBuffer 通道 + PBR 物理意义）| `Task01_PBRShowcase/` |
| T02 | 岩浆地面 UV 动画（TexCoord / Panner）| `Task02_LavaUV/` |
| T03 | Fresnel Material Function（封装与复用）| `Task03_FresnelMF/` |
| T04 | Weather MPC（全局参数 + 运行时驱动）| `Task04_WeatherMPC/` |

完成后能回答：
- Master Material 与 Material Instance 的编译开销差别？
- GBuffer 有哪些通道？Buffer Visualization 怎么开？
- TexCoord/Panner 是 VS 还是 PS 阶段？
- MF vs MPC vs Custom Node 三种"代码复用"的边界？

## T01 - PBR 材质球展厅

**步骤：**
1. 创建 Master Material `M_PBRMaster`，把 BaseColor / Metallic / Roughness / Specular / Emissive / Opacity 全部转成 Scalar Parameter / Vector Parameter
2. 右键 `M_PBRMaster` → Create Material Instance ×6，分别命名 `MI_Metallic / MI_Roughness / ...`
3. 关卡里放 6 个 Sphere，每个 Sphere 赋一种 MI
4. **Buffer Visualization**：Viewport → 视图模式 → Buffer Visualization → 选 BaseColor / Metallic / Normal / Roughness 等观察 GBuffer 输出
5. 第 6 个球用 `MI_Translucent`（Material Domain: Translucent，BlendMode: Translucent）—— 注意此球**不写 GBuffer**

**关键认知：**
- 前 5 球走 **Deferred** 管线：BaseColor/Metallic/Roughness/Normal/Specular 各自写入 GBuffer 通道
- 第 6 球走 **Forward Translucent** 通道：每像素独立采样光源，与 Deferred 完全不同的成本
- **Material Instance** 不编译新 shader，只改 uniform 参数；改 Static Switch / Feature Level 才会触发新 permutation 编译

## T02 - 岩浆地面 UV 动画

**步骤：**
1. 准备两张噪声纹理 `T_NoiseA` / `T_NoiseB`
2. 在材质里：`TexCoord[0]` → `Panner(Speed=Vector(0.05, 0.02))` → 采样 `T_NoiseA`
3. 同样路径但 Speed=(−0.03, 0.04) 采样 `T_NoiseB`
4. `Multiply(NoiseA, NoiseB)` → 当作 Emissive 强度
5. 用第三张纹理 R 通道 → `Lerp(BaseColor_Cooled, BaseColor_Lava, R)` → 输出 BaseColor
6. 把 PannerSpeed 暴露为 `Vector Parameter`

**关键认知：**
- `TexCoord[0]` 是 mesh UV0，每顶点一个 float2，Panner 在 **Vertex Shader** 加 Time*Speed 偏移
- Sampler Source 设为 **Shared: Wrap** 节省 sampler slot（SM5/SM6 限制 16 个独立 sampler）
- 多层 UV 动画叠加 = 节点图就够，**不需要任何代码**

## T03 - Material Function：可复用 Fresnel 描边

**步骤：**
1. New → Material → Material Function → 命名 `MF_FresnelRim`
2. 内部加 `FunctionInput RimColor (Vector3)` + `FunctionInput RimPower (Scalar, default 4)`
3. 实现：`Fresnel(Power=RimPower)` → `Multiply(RimColor)` → `FunctionOutput`
4. 在三个完全不同的材质里调用 `MF_FresnelRim`：角色皮肤（边缘加暖色）/ 能量护盾（边缘亮蓝）/ UI 元素（白色高亮）

**关键认知：**
- MF 是 shader 代码复用单元——编译时 **inline 展开** 到调用方 shader（不是运行时函数调用）
- 输入输出对应 HLSL 函数签名概念
- **MF 的边界**：涉及多 pass / RenderTarget 读写 / Compute 时，MF 不够，必须 Global Shader / Custom Node

## T04 - Material Parameter Collection：全局天气

**步骤：**
1. New → Material → Material Parameter Collection → `MPC_Weather`
2. 添加 Scalar：`Wetness` (default 0)、`RainIntensity` (default 0)；Vector：`WindDirection` (default 1,0,0,0)
3. 在地面 / 树叶 / 建筑材质里用 `CollectionParameter` 节点读 `MPC_Weather.Wetness` → `Lerp(0.8, 0.2, Wetness)` → 输出 Roughness
4. 关卡蓝图（或本目录 `T04_WeatherMPCHelper`）：调 `SetWetness(0.5)` → 全场材质同步变化

**关键认知：**
- MPC 在 GPU 端是**单个 Uniform Buffer**，一次更新影响所有引用材质
- 上限：1024 Scalar + 1024 Vector，**无 Texture 类型**
- **C++ vs 蓝图驱动**：BP 调 `SetScalarParameterValue` 简单，C++ 性能可控

## C++ Helper（仅 T04 提供）

```cpp
UT04_WeatherMPCHelper::SetWetness(this, MPC_Weather, 0.5f);
UT04_WeatherMPCHelper::SetRainIntensity(this, MPC_Weather, 1.0f);
UT04_WeatherMPCHelper::SetWindDirection(this, MPC_Weather, FVector(0, 1, 0));
```

实际项目里会接到天气系统 / 时间系统 → 平滑插值更新 MPC。

## 测试方法

**手动 PIE：**
- T01：Buffer Visualization 看到 5 个球的 GBuffer 输出符合预期；Translucent 球 Buffer Visualization Metallic 为 0（不写 GBuffer）
- T02：观察纹理流动 + 混合的"流动感"
- T03：三个材质的 Fresnel 边缘颜色不同，但代码（MF 实现）只一处
- T04：调 Wetness 一次，地面/树叶/建筑同步变湿

**自动化测试：** 此阶段为编辑器操作，无 Spec 覆盖（材质资产无法在 NewObject 中验证视觉效果）。

## 常见坑
- T01：MI 改了"Static Switch" 触发了新 permutation 编译，shader 编译成本 ≈ 新 Master Material
- T02：忘了把 Panner Speed 暴露为参数 → MI 改不了
- T03：MF 编辑器关掉后忘了 Apply → 调用方未生效
- T04：MPC 不能放 Texture 参数；想要"全局贴图"用单独的 Streaming Virtual Texture 或 Global Texture
