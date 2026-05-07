// T15 - Compute Shader ：GPU 粒子模拟
//
// 关键点 ：
//   * 没有 VS/PS ，只有 CS（SF_Compute）
//   * 通过 RWStructuredBuffer 读写粒子数据
//   * UE5 推荐用 RDG (Render Dependency Graph) 而不是直接 RHI Command List
//   * RDG 自动处理 ：资源屏障 / 生命周期 / pass 排序

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphResources.h"

// 与 .usf 内 FParticleData 一致 ：16 字节对齐
struct FT15_ParticleData
{
	FVector3f Position;
	float     _Pad0 = 0.f;
	FVector3f Velocity;
	float     _Pad1 = 0.f;
};

class FT15_ParticleSimCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FT15_ParticleSimCS);
	SHADER_USE_PARAMETER_STRUCT(FT15_ParticleSimCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FT15_ParticleData>, ParticleBuffer)
		SHADER_PARAMETER(uint32, ParticleCount)
		SHADER_PARAMETER(float, DeltaTime)
		SHADER_PARAMETER(float, Gravity)
	END_SHADER_PARAMETER_STRUCT()

	// 与 [numthreads(64,1,1)] 保持一致
	static constexpr int32 ThreadGroupSize = 64;

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
	{
		// CS 通常 SM5+
		return IsFeatureLevelSupported(Params.Platform, ERHIFeatureLevel::SM5);
	}

	// 把 ThreadGroupSize 暴露给 .usf 编译器（如想在 HLSL 用宏）
	static void ModifyCompilationEnvironment(
		const FGlobalShaderPermutationParameters& Params,
		FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Params, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("THREADGROUP_SIZE"), ThreadGroupSize);
	}
};
