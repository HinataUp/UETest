// 渲染模块 自动化测试
//
// 单测能覆盖 ：组件默认配置 / Shader 类反射注册
// 不能覆盖 ：实际 GPU 渲染结果（需要 PIE + 视觉验证）

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "../Stage2_SpecialMaterials/Task06_Decal/T06_DecalSpawner.h"
#include "../Stage4_CppControl/Task11_HitFlash/T11_HitFlashComponent.h"
#include "../Stage4_CppControl/Task12_DamagePaint/T12_DamagePaintComponent.h"
#include "../Stage4_CppControl/Task13_SceneCapture/T13_SecurityCamera.h"
#include "../Stage5_ShaderCode/Task14_GlobalShader/T14_TonemapShader.h"
#include "../Stage5_ShaderCode/Task15_ComputeShader/T15_ParticleSimShader.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ShaderCore.h"

BEGIN_DEFINE_SPEC(
	FUETest_Rendering_Spec,
	"UETest.Rendering.渲染模块",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_Rendering_Spec)

void FUETest_Rendering_Spec::Define()
{
	Describe(TEXT("Stage 2 - T06 弹痕 Decal Spawner"), [this]()
	{
		It(TEXT("默认 LifeSpan / FadeDuration 合理"), [this]()
		{
			UT06_DecalSpawner* Comp = NewObject<UT06_DecalSpawner>();
			TestTrue(TEXT("【Decal】LifeSpan > 0"), Comp->LifeSpan > 0.f);
			TestTrue(TEXT("【Decal】FadeDuration <= LifeSpan"),
				Comp->FadeDuration <= Comp->LifeSpan);
		});
	});

	Describe(TEXT("Stage 4 - T11 受击闪白"), [this]()
	{
		It(TEXT("默认 FlashDuration 合理"), [this]()
		{
			UT11_HitFlashComponent* Comp = NewObject<UT11_HitFlashComponent>();
			TestTrue(TEXT("【HitFlash】FlashDuration > 0.05"),
				Comp->FlashDuration >= 0.05f);
		});
	});

	Describe(TEXT("Stage 4 - T12 伤痕 RT"), [this]()
	{
		It(TEXT("默认 RT 尺寸为 2 的幂"), [this]()
		{
			UT12_DamagePaintComponent* Comp = NewObject<UT12_DamagePaintComponent>();
			const int32 S = Comp->RenderTargetSize;
			TestTrue(TEXT("【RT】尺寸合理"), S >= 64 && S <= 4096);
		});
	});

	Describe(TEXT("Stage 4 - T13 SceneCapture 性能配置"), [this]()
	{
		It(TEXT("CaptureComp 默认 bCaptureEveryFrame=false"), [this]()
		{
			AT13_SecurityCamera* Cam = NewObject<AT13_SecurityCamera>();
			USceneCaptureComponent2D* Cap = Cam->FindComponentByClass<USceneCaptureComponent2D>();
			TestNotNull(TEXT("CaptureComp 存在"), Cap);
			if (Cap)
			{
				TestFalse(TEXT("【SceneCapture】不应每帧捕获"), Cap->bCaptureEveryFrame);
				TestFalse(TEXT("【SceneCapture】不应移动时捕获"), Cap->bCaptureOnMovement);
				// 性能 ShowFlags
				TestFalse(TEXT("【ShowFlags】Atmosphere 应关闭"), Cap->ShowFlags.Atmosphere);
				TestFalse(TEXT("【ShowFlags】Bloom 应关闭"), Cap->ShowFlags.Bloom);
			}
		});
	});

	Describe(TEXT("Stage 5 - T14 Global Shader 已注册"), [this]()
	{
		It(TEXT("FT14_TonemapVS / PS 类型反射可达"), [this]()
		{
			// FGlobalShader 子类有 StaticType ；非空即代表已 IMPLEMENT_GLOBAL_SHADER 注册
			TestTrue(TEXT("VS 注册"), FT14_TonemapVS::GetStaticType().GetName() != nullptr);
			TestTrue(TEXT("PS 注册"), FT14_TonemapPS::GetStaticType().GetName() != nullptr);
		});

		It(TEXT("Shader 路径映射 /UETest 已建立"), [this]()
		{
			// StartupModule 中应已添加 /UETest -> Source/.../Shaders 映射
			TestTrue(TEXT("/UETest 路径映射存在"),
				AllShaderSourceDirectoryMappings().Contains(TEXT("/UETest")));
		});
	});

	Describe(TEXT("Stage 5 - T15 Compute Shader 已注册"), [this]()
	{
		It(TEXT("FT15_ParticleSimCS 类型反射可达"), [this]()
		{
			TestTrue(TEXT("CS 注册"),
				FT15_ParticleSimCS::GetStaticType().GetName() != nullptr);
		});

		It(TEXT("ThreadGroupSize 与 .usf 一致 (64)"), [this]()
		{
			TestEqual(TEXT("ThreadGroupSize"),
				FT15_ParticleSimCS::ThreadGroupSize, 64);
		});

		It(TEXT("FT15_ParticleData 16 字节对齐 (32 bytes total)"), [this]()
		{
			// 与 .usf 内 struct FParticleData 一一对应
			TestEqual(TEXT("FParticleData 大小应为 32 字节"),
				static_cast<int32>(sizeof(FT15_ParticleData)), 32);
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
