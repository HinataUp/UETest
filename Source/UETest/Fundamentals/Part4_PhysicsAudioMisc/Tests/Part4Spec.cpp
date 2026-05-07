// Part 4 - 物理 / 音频 / 特效 / 工程补充 自动化测试

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "../A_Physics/P86_CollisionShowcaseActor.h"
#include "../A_Physics/P87_TraceHelper.h"
#include "../A_Physics/P88_PhysicsCubeActor.h"

#include "../B_Niagara/P89_NiagaraSpawner.h"

#include "../C_Audio/P91_92_AudioHelper.h"

#include "../D_Sequencer/P93_SequencePlayer.h"
#include "../D_Sequencer/P95_MaterialMPCHelper.h"
#include "../D_Sequencer/P96_RenderTargetHelper.h"

#include "../E_EditorPackaging/P97_BatchActorHelper.h"
#include "../E_EditorPackaging/P98_BuildInfoHelper.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

BEGIN_DEFINE_SPEC(
	FUETest_FundamentalsP4_Spec,
	"UETest.Fundamentals.P4_物理音频特效",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_FundamentalsP4_Spec)

void FUETest_FundamentalsP4_Spec::Define()
{
	Describe(TEXT("A_物理碰撞"), [this]()
	{
		It(TEXT("P86 SphereComp + 默认配置"), [this]()
		{
			AP86_CollisionShowcaseActor* A = NewObject<AP86_CollisionShowcaseActor>();
			TestNotNull(TEXT("SphereComp"), A->SphereComp.Get());
			TestEqual(TEXT("ObjectType=WorldDynamic"),
				static_cast<int32>(A->SphereComp->GetCollisionObjectType()),
				static_cast<int32>(ECC_WorldDynamic));
			TestTrue(TEXT("生成 Overlap 事件"), A->SphereComp->GetGenerateOverlapEvents());
		});

		It(TEXT("P87 TraceHelper 是 BP Function Library"), [this]()
		{
			TestTrue(TEXT("继承 UBlueprintFunctionLibrary"),
				UP87_TraceHelper::StaticClass()->IsChildOf(UBlueprintFunctionLibrary::StaticClass()));
		});

		It(TEXT("P88 物理 Cube 默认 Simulate=true + Movable"), [this]()
		{
			AP88_PhysicsCubeActor* C = NewObject<AP88_PhysicsCubeActor>();
			TestNotNull(TEXT("CubeMesh"), C->CubeMesh.Get());
			TestTrue(TEXT("SimulatePhysics=true"), C->CubeMesh->IsSimulatingPhysics());
			TestEqual(TEXT("Movable"),
				static_cast<int32>(C->CubeMesh->Mobility),
				static_cast<int32>(EComponentMobility::Movable));
			TestTrue(TEXT("ImpulseStrength > 0"), C->ImpulseStrength > 0.f);
		});
	});

	Describe(TEXT("B_Niagara"), [this]()
	{
		It(TEXT("P89 NiagaraSpawner 是 Actor Component"), [this]()
		{
			TestTrue(TEXT("ActorComponent 子类"),
				UP89_NiagaraSpawner::StaticClass()->IsChildOf(UActorComponent::StaticClass()));
			TestNotNull(TEXT("OnExplosionFinished 委托可达"),
				UP89_NiagaraSpawner::StaticClass()->FindPropertyByName(TEXT("OnExplosionFinished")));
		});
	});

	Describe(TEXT("C_音频"), [this]()
	{
		It(TEXT("P91-92 AudioHelper 静态接口反射可达"), [this]()
		{
			UClass* C = UP91_92_AudioHelper::StaticClass();
			TestNotNull(TEXT("Play3DSound"),  C->FindFunctionByName(TEXT("Play3DSound")));
			TestNotNull(TEXT("Play2DSound"),  C->FindFunctionByName(TEXT("Play2DSound")));
			TestNotNull(TEXT("TriggerMetaSoundParameter"),
				C->FindFunctionByName(TEXT("TriggerMetaSoundParameter")));
		});
	});

	Describe(TEXT("D_Sequencer与材质"), [this]()
	{
		It(TEXT("P93 SequencePlayerHelper 是 BP Function Library"), [this]()
		{
			TestTrue(TEXT("BP Function Library 子类"),
				UP93_SequencePlayerHelper::StaticClass()->IsChildOf(UBlueprintFunctionLibrary::StaticClass()));
		});

		It(TEXT("P95 MaterialMPCHelper 是 BP Function Library"), [this]()
		{
			TestTrue(TEXT("BP Function Library 子类"),
				UP95_MaterialMPCHelper::StaticClass()->IsChildOf(UBlueprintFunctionLibrary::StaticClass()));
		});

		It(TEXT("P96 RenderTargetHelper 静态接口反射可达"), [this]()
		{
			UClass* C = UP96_RenderTargetHelper::StaticClass();
			TestNotNull(TEXT("CreateRT"), C->FindFunctionByName(TEXT("CreateRT")));
			TestNotNull(TEXT("DrawCircleOnRT"), C->FindFunctionByName(TEXT("DrawCircleOnRT")));
			TestNotNull(TEXT("ClearRT"), C->FindFunctionByName(TEXT("ClearRT")));
		});
	});

	Describe(TEXT("E_编辑器与打包"), [this]()
	{
		It(TEXT("P97 AddTagToActors 行为正确"), [this]()
		{
			TArray<AActor*> Actors;
			AActor* A1 = NewObject<AActor>();
			AActor* A2 = NewObject<AActor>();
			Actors.Add(A1); Actors.Add(A2);
			const int32 N = UP97_BatchActorHelper::AddTagToActors(Actors, FName("Test"));
			TestEqual(TEXT("处理了 2 个"), N, 2);
			TestTrue(TEXT("A1 含 Test Tag"), A1->Tags.Contains(FName("Test")));
		});

		It(TEXT("P98 GetBuildConfiguration 不为 Unknown"), [this]()
		{
			const EP98_BuildConfig C = UP98_BuildInfoHelper::GetBuildConfiguration();
			TestTrue(TEXT("非 Unknown"),
				C != EP98_BuildConfig::Unknown);
		});

		It(TEXT("P98 IsEditorBuild + HasDebugOutput 配对正确"), [this]()
		{
			// 跑测试时一定是编辑器 ；HasDebugOutput=true（非 Shipping）
			TestTrue(TEXT("Editor 构建运行测试"),
				UP98_BuildInfoHelper::IsEditorBuild());
			TestFalse(TEXT("非 Shipping"),
				UP98_BuildInfoHelper::IsShippingBuild());
			TestTrue(TEXT("有调试输出"),
				UP98_BuildInfoHelper::HasDebugOutput());
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
