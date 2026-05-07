# Stage 5 - 深入 Shader 代码（T14-T15 💻 必须 C++ + HLSL）

## 模块总览
脱离 Material Editor，直接编写 `.usf` shader 文件。代码在引擎启动时编译成 GPU shader binary。

| Task | 主题 | 文件 |
| --- | --- | --- |
| T14 | Global Shader（自定义全屏 ACES Tonemap）| `Task14_GlobalShader/T14_TonemapShader` |
| T15 | Compute Shader（GPU 粒子模拟 + RDG）| `Task15_ComputeShader/T15_ParticleSimShader / Component` |

完成后能回答：
- Material vs Global Shader 的边界？
- Compute Shader 与 VS/PS 的区别？
- RDG 解决了什么？

## 前置工程化

**Build.cs 依赖：**
```csharp
PublicDependencyModuleNames.AddRange(new[] {
    "RenderCore", "RHI", "Renderer", "Projects"
});
```

**Shader 路径注册（StartupModule）：**
```cpp
const FString RealShaderDir = FPaths::Combine(
    FPaths::ProjectDir(), TEXT("Source/UETest/Rendering/Shaders"));
AddShaderSourceDirectoryMapping(TEXT("/UETest"), RealShaderDir);
// 之后 .usf 中 #include "/UETest/..." 能找到对应文件
```

见本项目 `Source/UETest/UETest.cpp` 的 `FUETestModule::StartupModule()`。

## T14 - Global Shader：ACES Tonemap

**.usf 文件**（`Shaders/Private/MyTonemap.usf`）：
- VS：用 `SV_VertexID` 0/1/2 直接生成覆盖 NDC 的全屏三角形（无需 VB）
- PS：采样 SceneColor → ACES 简化公式 → 与原色按 Strength lerp

**C++ 类（FGlobalShader 子类）：**
```cpp
class FT14_TonemapPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FT14_TonemapPS);
    SHADER_USE_PARAMETER_STRUCT(FT14_TonemapPS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTex)
        SHADER_PARAMETER_SAMPLER(SamplerState, SceneColorSampler)
        SHADER_PARAMETER(float, Strength)
        SHADER_PARAMETER(FVector4f, OutputViewportSize)
        RENDER_TARGET_BINDING_SLOTS()
    END_SHADER_PARAMETER_STRUCT()
};

IMPLEMENT_GLOBAL_SHADER(FT14_TonemapPS,
    "/UETest/Private/MyTonemap.usf", "MainPS", SF_Pixel);
```

**实战调用方式（本 task 未提供完整 ViewExtension，原理）：**
```cpp
class FMyTonemapViewExtension : public FSceneViewExtensionBase
{
    virtual void SubscribeToPostProcessingPass(
        EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOut, bool bIsPassEnabled) override
    {
        if (Pass == EPostProcessingPass::Tonemap)
        {
            InOut.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FMyTonemapViewExtension::AfterTonemap));
        }
    }
    // AfterTonemap ：构建 RDG，用 FT14_TonemapPS 做一次全屏 pass
};
```

**关键认知：**
- Global Shader = "引擎级 shader" ：不依赖 Material/MI，直接编译自 `.usf`
- 编译时机 ：引擎启动 / cook 时（不是运行时），`IMPLEMENT_GLOBAL_SHADER` 宏注册到全局表
- Material Editor 是在引擎预定义 shader template 里"填参数"；**Global Shader 是你自己定义整个 shader**

## T15 - Compute Shader：GPU 粒子模拟

**.usf 文件**（`Shaders/Private/ParticleSim.usf`）：
```hlsl
struct FParticleData {
    float3 Position; float _Pad0;
    float3 Velocity; float _Pad1;
};

RWStructuredBuffer<FParticleData> ParticleBuffer;
uint  ParticleCount;
float DeltaTime;
float Gravity;

[numthreads(64, 1, 1)]
void MainCS(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= ParticleCount) return;
    FParticleData P = ParticleBuffer[DTid.x];
    P.Velocity.z -= Gravity * DeltaTime;
    P.Position += P.Velocity * DeltaTime;
    if (P.Position.z < 0) {
        P.Position.z = 0;
        P.Velocity.z = -P.Velocity.z * 0.5;
    }
    ParticleBuffer[DTid.x] = P;
}
```

**C++ 调度（用 RDG）：**
```cpp
ENQUEUE_RENDER_COMMAND(T15_ParticleSim)(
    [Count, G, Dt](FRHICommandListImmediate& RHICmdList)
    {
        FRDGBuilder GraphBuilder(RHICmdList);

        FRDGBufferRef BufferRDG = GraphBuilder.CreateBuffer(
            FRDGBufferDesc::CreateStructuredDesc(sizeof(FT15_ParticleData), Count),
            TEXT("ParticleBuffer"));

        FT15_ParticleSimCS::FParameters* Params = GraphBuilder.AllocParameters<...>();
        Params->ParticleBuffer = GraphBuilder.CreateUAV(BufferRDG);
        Params->ParticleCount = Count;
        // ...

        TShaderMapRef<FT15_ParticleSimCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("ParticleSim"),
            CS, Params, FIntVector(GroupCount, 1, 1));

        GraphBuilder.Execute();
    });
```

**关键认知：**
- Compute Shader 不走光栅化 ：没有 VS/PS，纯并行数据处理
- 通过 `[numthreads(X,Y,Z)]` 声明线程组大小 ；64 是大多数 GPU 的 wave 倍数，性能最佳
- `RWStructuredBuffer` 是 GPU 端"数组" ：CPU 上传 → CS 读写 → 给 ISM/Niagara 用
- **强烈推荐 RDG**（Render Dependency Graph）：自动处理资源屏障 / 生命周期 / pass 排序

## 测试方法

**自动化测试：** `UETest.Rendering.渲染模块`
```
Stage 5 - T14 Global Shader 已注册
  - VS / PS 类型反射可达
  - Shader 路径映射 /UETest 已建立
Stage 5 - T15 Compute Shader 已注册
  - CS 类型反射可达
  - ThreadGroupSize 与 .usf 一致 (64)
  - FT15_ParticleData 16 字节对齐 (32 bytes)
```

**手动 PIE：**
- T14 ：实际接入需写 ViewExtension（本 task 未提供，是教学骨架）
- T15 ：调 `InitParticles(1000) + Simulate(0.016)` ：粒子在 GPU 上模拟（debug 时用 RenderDoc 抓帧看 dispatch）

## 常见坑
- 忘了 `AddShaderSourceDirectoryMapping` → `.usf` 找不到，编译报错
- Build.cs 漏 `RenderCore/Renderer/RHI/Projects` → 链接错
- Shader 参数 struct 字段顺序与 `.usf` 不一致 → 数据错乱
- HLSL struct 不 16 字节对齐 → GPU 端读到错位数据
- Compute Shader 没考虑 Count 不是 64 整数倍 → 越界写 → 崩
- 直接用 RHI Command List 而不是 RDG → 自己处理 barrier 容易写错

## 下一步学习

- **完整 ViewExtension** 接管 Tonemap pass → 看 Lyra `LyraDebugCameraController` 风格
- **粒子回读到 Niagara** ：通过 Niagara DataInterface 把 GPU buffer 接到 ParticleEmitter
- **MaterialShader vs GlobalShader** ：Material 是基于材质的 shader，VS/PS 都来自 Material 节点 + 引擎模板
