# UE5.6 Shader & 渲染学习模块

## 总览
15 个不重叠的小任务，从纯编辑器可视化操作到必须写 C++/HLSL，建立"哪些事用编辑器做、哪些事必须写代码"的判断力。

## 阶段地图

| 阶段 | 任务 | 主题 | 实现方式 | 文件夹 |
| --- | --- | --- | --- | --- |
| **Stage 1** | T01-T04 | Material Editor 基础 | 🎨 编辑器 | `Stage1_MaterialBasics/` |
| **Stage 2** | T05-T07 | 特殊材质类型 | 🔀 编辑器+少量C++ | `Stage2_SpecialMaterials/` |
| **Stage 3** | T08-T10 | Custom HLSL 文本 | 🔀 Material Editor 内写 HLSL | `Stage3_CustomHLSL/` |
| **Stage 4** | T11-T13 | C++ 运行时控制 | 💻🎨 C++ 主导 | `Stage4_CppControl/` |
| **Stage 5** | T14-T15 | Global / Compute Shader | 💻 必须 C+++HLSL | `Stage5_ShaderCode/` |
| 笔记 | T16 | 渲染管线决策图 | 📝 见下方 | 本 README |

## 任务一览表

| # | Task | 核心学习点 | 方式 |
| --- | --- | --- | --- |
| 01 | PBR 材质球展厅 | GBuffer 通道、PBR 物理意义 | 🎨 |
| 02 | 岩浆地面 UV 动画 | TexCoord、Panner、时间驱动 | 🎨 |
| 03 | Fresnel MF 复用 | Material Function 封装机制 | 🎨 |
| 04 | 全局天气 MPC | Material Parameter Collection | 🎨🔀 |
| 05 | 全屏灰度暗角 PP | Post-Process Material、屏幕空间 | 🎨🔀 |
| 06 | 弹痕 Decal | Deferred Decal、DBuffer 管线 | 🎨🔀 |
| 07 | X-Ray 描边 | Custom Depth/Stencil | 🔀 |
| 08 | Dissolve Custom Node | Material Editor 内 HLSL | 🔀 |
| 09 | 旗帜 WPO 飘动 | Vertex Shader (WPO) | 🎨 |
| 10 | POM 深度欺骗 | Pixel Depth Offset、ray marching | 🎨🔀 |
| 11 | 受击闪白 MID | C++ 运行时材质参数控制 | 💻🎨 |
| 12 | 伤痕 RT 绘制 | Render Target 读写 | 💻🎨 |
| 13 | 安全摄像头画中画 | Scene Capture + RT | 💻🎨 |
| 14 | Global Shader 全屏 pass | .usf 文件、FGlobalShader | 💻 |
| 15 | Compute Shader 粒子 | GPU Compute、RDG、StructuredBuffer | 💻 |
| 16 | 渲染管线决策图 | 全局架构认知 | 📝 |

## 工程化

### 已配置
- `Source/UETest/UETest.Build.cs` 添加 `RenderCore / RHI / Renderer / Projects` 依赖
- `Source/UETest/UETest.cpp` 用 `FUETestModule::StartupModule()` 注册 shader 目录映射 `/UETest -> Source/UETest/Rendering/Shaders`

### Shader 目录约定
```
Source/UETest/Rendering/Shaders/Private/*.usf  → /UETest/Private/*.usf 在 .cpp 中引用
```

## T16 - 渲染管线决策图

完成所有任务后，建立这张"应用层 ↔ GPU 管线"对应关系：

```
应用层 CPU                                 GPU 渲染管线
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

GameThread                                 
  ├─ MID.SetParameter()         🎨💻 ──→  Uniform Buffer 更新（T11）
  ├─ MPC.SetValue()             🎨🔀 ──→  全局 Uniform Buffer 更新（T04）
  ├─ SceneCapture.CaptureScene  💻   ──→  额外一次完整管线（T13）
  └─ DrawToRenderTarget         💻🎨 ──→  一次全屏 pass（T12）

Vertex Shader 阶段:
  Material Editor 的 WPO 通道   🎨   ──→  顶点位置偏移（T09）
  Custom Node 内 HLSL（VS-side）🔀  ──→  顶点级精控

Pixel Shader 阶段（GBuffer 写入）:
  Material BaseColor/Normal/
    Metallic/Roughness          🎨   ──→  GBuffer 各通道（T01）
  Decal Material（DBuffer）     🎨🔀 ──→  GBuffer 注入（T06）
  Custom Node Pixel HLSL        🔀   ──→  Dissolve / 自定义算法（T08）
  Pixel Depth Offset            🎨🔀 ──→  深度欺骗（T10）

Lighting 阶段:
  引擎内置                     ──────→  Lumen / VSM / SSR（不直接介入）

Post-Process 阶段:
  Post-Process Material         🎨🔀 ──→  全屏像素操作（T05）
  Custom Depth/Stencil          🔀   ──→  PP 材质采样辅助 buffer（T07）
  Global Shader                 💻   ──→  自定义全屏 pass（T14）
    （FSceneViewExtension）

Compute（并行管线）:
  Compute Shader + RDG          💻   ──→  通用 GPU 数据处理（T15）

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## 决策树："应该用哪个工具？"

```
你想做的事情
│
├─ 改材质长什么样
│  │
│  ├─ 简单 PBR 参数？        → Material Editor + MI（T01-04）
│  ├─ 多层 UV 动画？         → Material Editor + Panner（T02）
│  ├─ 复用一段逻辑到多材质？ → Material Function（T03）
│  ├─ 跨材质共享变量？        → Material Parameter Collection（T04）
│  └─ Material Editor 节点不够？ → Custom Node 写 HLSL（T08）
│
├─ 改顶点位置
│  │
│  ├─ 周期 / 程序化变形？    → World Position Offset（T09）
│  └─ 物理 / 数据驱动变形？  → C++ 修改顶点（ProceduralMeshComponent）
│
├─ 屏幕空间效果
│  │
│  ├─ 简单后处理？           → Post-Process Material（T05）
│  ├─ 区分物体类别？         → Custom Stencil + PP Material（T07）
│  └─ 完全自定义全屏 pass？  → Global Shader（T14）
│
├─ 动态修改材质参数（运行时）
│  │
│  ├─ 单个 actor 的视觉变化？ → Dynamic Material Instance（T11）
│  └─ 全局变化？             → MPC.SetValue（T04）
│
├─ 渲染到纹理
│  │
│  ├─ 想"画"东西到 RT？      → DrawMaterialToRenderTarget（T12）
│  └─ 想把场景渲染到 RT？    → SceneCapture（T13）
│
└─ 大规模 GPU 计算
    │
    └─ 粒子模拟 / 数据处理？  → Compute Shader + RDG（T15）
```

## 自动化测试

`UETest.Rendering.渲染模块` —— 覆盖配置正确性 + Shader 类反射注册（详见 `Tests/RenderingSpec.cpp`）。

视觉效果 / GPU 实际行为必须靠 PIE 验证；详见各 stage README 的"测试方法"段。

## 学习路径建议

按 v2 文档推荐的节奏：
- **Week 1-2**：T01 → T02 → T03 → T04（Material Editor 手感）
- **Week 3**：T05 → T06 → T07（特殊材质类型）
- **Week 4**：T08 → T09 → T10（Custom HLSL 入门）
- **Week 5**：T11 → T12 → T13（C++ 运行时控制）
- **Week 6+**：T14 → T15 → T16（深水区：Global / Compute Shader）

每个任务做完写一句话："这一步是编辑器做的、这一步必须写代码、为什么"——边界感的积累比技术细节更值钱。
