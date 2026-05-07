// T15 - Compute Shader 调度实现
//
// 流程 ：
//   1. 在 GameThread 上准备 SRV 数据 + 设定参数
//   2. ENQUEUE_RENDER_COMMAND 推到 RenderThread
//   3. RenderThread 上构建 RDG ：
//      - CreateBuffer + Upload 初始数据
//      - AddPass 调用 CS
//      - 资源生命周期由 RDG 自动管理

#include "T15_ParticleSimComponent.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "GlobalShader.h"
#include "RHICommandList.h"
#include "../../00_Common/RenderingLogMacros.h"

UT15_ParticleSimComponent::UT15_ParticleSimComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UT15_ParticleSimComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UT15_ParticleSimComponent::InitParticles(int32 NumParticles)
{
	CPUParticles.Reset();
	CPUParticles.Reserve(NumParticles);

	// 初始 ：在原点附近随机散布 ，初速向上偏 + 随机水平
	for (int32 i = 0; i < NumParticles; ++i)
	{
		FT15_ParticleData P;
		P.Position = FVector3f(
			FMath::RandRange(-50.f, 50.f),
			FMath::RandRange(-50.f, 50.f),
			FMath::RandRange(0.f, 100.f));
		P.Velocity = FVector3f(
			FMath::RandRange(-100.f, 100.f),
			FMath::RandRange(-100.f, 100.f),
			FMath::RandRange(200.f, 500.f));
		CPUParticles.Add(P);
	}
	CachedParticleCount = NumParticles;
	RT_LOG("ComputeSim 初始化 ：%d 粒子", NumParticles);
}

void UT15_ParticleSimComponent::Simulate(float DeltaSeconds)
{
	if (CachedParticleCount <= 0) { return; }

	const int32 Count = CachedParticleCount;
	const float G = Gravity;
	const float Dt = FMath::Clamp(DeltaSeconds, 0.f, 0.1f);

	// 把 CPU 端粒子初始数据复制一份给 RenderThread（首帧上传）
	// 简化教学版 ：每次都重新上传初始数据 ；真实项目应该只首帧上传 ，之后保持 GPU 端 buffer
	TArray<FT15_ParticleData> InitialDataCopy = CPUParticles;

	ENQUEUE_RENDER_COMMAND(T15_ParticleSim)(
		[Count, G, Dt, InitialData = MoveTemp(InitialDataCopy)](FRHICommandListImmediate& RHICmdList) mutable
		{
			FRDGBuilder GraphBuilder(RHICmdList);

			// 1. 创建 RDG StructuredBuffer
			const int32 BufferSize = sizeof(FT15_ParticleData) * Count;
			FRDGBufferRef ParticleRDG = GraphBuilder.CreateBuffer(
				FRDGBufferDesc::CreateStructuredDesc(sizeof(FT15_ParticleData), Count),
				TEXT("T15_ParticleBuffer"));

			// 2. 上传初始数据
			GraphBuilder.QueueBufferUpload(
				ParticleRDG, InitialData.GetData(), BufferSize, ERDGInitialDataFlags::None);

			// 3. 配置 shader 参数
			TShaderMapRef<FT15_ParticleSimCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));

			FT15_ParticleSimCS::FParameters* Params = GraphBuilder.AllocParameters<FT15_ParticleSimCS::FParameters>();
			Params->ParticleBuffer = GraphBuilder.CreateUAV(ParticleRDG);
			Params->ParticleCount = Count;
			Params->DeltaTime = Dt;
			Params->Gravity = G;

			// 4. dispatch CS ：组数 = ceil(Count / ThreadGroupSize)
			const FIntVector GroupCount(
				FMath::DivideAndRoundUp(Count, FT15_ParticleSimCS::ThreadGroupSize), 1, 1);
			FComputeShaderUtils::AddPass(
				GraphBuilder,
				RDG_EVENT_NAME("T15_ParticleSim"),
				CS, Params, GroupCount);

			// 5. RDG 执行 ：自动处理 barrier
			GraphBuilder.Execute();
		});
}
