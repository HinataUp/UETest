// T14 - Global Shader ：自定义色调映射全屏 pass
//
// Global Shader = 不依赖任何 Material 的引擎级 shader 。直接编译 .usf 。
// 注册时机 ：引擎启动 / cook 时（不是运行时） ；用 IMPLEMENT_GLOBAL_SHADER 宏
// Build.cs 必须依赖 ：RenderCore + Renderer + RHI
//
// 本 task 只声明 VS/PS 类与参数 binding ；如何分发见对应 ViewExtension（略）。

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphResources.h"

// 全屏 VS ：通过 SV_VertexID 生成三角形，不需要 VB
class FT14_TonemapVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FT14_TonemapVS);
	SHADER_USE_PARAMETER_STRUCT(FT14_TonemapVS, FGlobalShader);

	// VS 没有外部参数（仅用 SV_VertexID） ；空 struct 占位
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
	{
		// 默认 SM5+ 才编译 ；移动端不需要
		return IsFeatureLevelSupported(Params.Platform, ERHIFeatureLevel::SM5);
	}
};

// PS ：采样 SceneColor -> ACES -> 与原色 lerp
class FT14_TonemapPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FT14_TonemapPS);
	SHADER_USE_PARAMETER_STRUCT(FT14_TonemapPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTex)
		SHADER_PARAMETER_SAMPLER(SamplerState, SceneColorSampler)
		SHADER_PARAMETER(float, Strength)
		SHADER_PARAMETER(FVector4f, OutputViewportSize)
		// 输出 RT 由 RDG 在外部声明 RenderTarget binding
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
	{
		return IsFeatureLevelSupported(Params.Platform, ERHIFeatureLevel::SM5);
	}
};
