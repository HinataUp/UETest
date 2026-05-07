// Part 3 - UI / AI / 动画 自动化测试
//
// 单测覆盖 ：类反射 / 默认配置 / BindWidget 字段反射可达
// 不覆盖 ：实际 UI 渲染 / AI 行为 / 动画播放 ：依赖 World + 资产 ；靠 PIE

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "../A_UI/P61_HUDWidget.h"
#include "../A_UI/P62_MenuWidget.h"
#include "../A_UI/P63_InventoryListView.h"
#include "../A_UI/P68_GameHUD.h"

#include "../B_AI/P70_BTTask_FindTarget.h"
#include "../B_AI/P70_BTDecorator_IsInRange.h"
#include "../B_AI/P72_PerceptionAIController.h"
#include "../B_AI/P73_NavMeshHelper.h"

#include "../C_Animation/P78_MyAnimInstance.h"
#include "../C_Animation/P80_AnimNotify_SendEvent.h"
#include "../C_Animation/P80_AnimNotifyState_TrailEffect.h"
#include "../C_Animation/P83_ControlRigHelper.h"

#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

BEGIN_DEFINE_SPEC(
	FUETest_FundamentalsP3_Spec,
	"UETest.Fundamentals.P3_UI_AI_动画",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_FundamentalsP3_Spec)

void FUETest_FundamentalsP3_Spec::Define()
{
	Describe(TEXT("A_UI"), [this]()
	{
		It(TEXT("P61 HUDWidget BindWidget 字段反射可达"), [this]()
		{
			// 反射查找 BindWidget 标记的 UPROPERTY
			FProperty* P = UP61_HUDWidget::StaticClass()->FindPropertyByName(TEXT("HealthText"));
			TestNotNull(TEXT("HealthText 反射可达"), P);
			TestNotNull(TEXT("HealthBar 反射可达"),
				UP61_HUDWidget::StaticClass()->FindPropertyByName(TEXT("HealthBar")));
		});

		It(TEXT("P62 MenuWidget OnStartGameRequested 委托可达"), [this]()
		{
			TestNotNull(TEXT("OnStartGameRequested 反射可达"),
				UP62_MenuWidget::StaticClass()->FindPropertyByName(TEXT("OnStartGameRequested")));
		});

		It(TEXT("P63 InventoryListView 子类正确"), [this]()
		{
			TestTrue(TEXT("Slot Widget 实现 IUserObjectListEntry"),
				UP63_InventorySlotWidget::StaticClass()->ImplementsInterface(UUserObjectListEntry::StaticClass()));
		});

		It(TEXT("P68 GameHUD 默认配置"), [this]()
		{
			AP68_GameHUD* HUD = NewObject<AP68_GameHUD>();
			TestTrue(TEXT("CrosshairSize > 0"), HUD->CrosshairSize > 0.f);
		});
	});

	Describe(TEXT("B_AI"), [this]()
	{
		It(TEXT("P70 BTTask_FindTarget 是 BTTaskNode 子类"), [this]()
		{
			TestTrue(TEXT("继承 UBTTaskNode"),
				UP70_BTTask_FindTarget::StaticClass()->IsChildOf(UBTTaskNode::StaticClass()));
		});

		It(TEXT("P70 BTDecorator 默认 Range 合理"), [this]()
		{
			UP70_BTDecorator_IsInRange* D = NewObject<UP70_BTDecorator_IsInRange>();
			TestTrue(TEXT("Range > 0"), D->Range > 0.f);
		});

		It(TEXT("P72 PerceptionAIController PerceptionComp 已创建"), [this]()
		{
			AP72_PerceptionAIController* PC = NewObject<AP72_PerceptionAIController>();
			TestNotNull(TEXT("PerceptionComp 不为空"), PC->PerceptionComp.Get());
			TestNotNull(TEXT("SightConfig 不为空"), PC->SightConfig.Get());
			TestTrue(TEXT("SightRadius > 0"), PC->SightConfig->SightRadius > 0.f);
		});

		It(TEXT("P73 NavMeshHelper 是 BlueprintFunctionLibrary"), [this]()
		{
			TestTrue(TEXT("继承 UBlueprintFunctionLibrary"),
				UP73_NavMeshHelper::StaticClass()->IsChildOf(UBlueprintFunctionLibrary::StaticClass()));
		});
	});

	Describe(TEXT("C_动画"), [this]()
	{
		It(TEXT("P78 AnimInstance 暴露 Speed/Direction/bIsInAir"), [this]()
		{
			UClass* C = UP78_MyAnimInstance::StaticClass();
			TestNotNull(TEXT("Speed"), C->FindPropertyByName(TEXT("Speed")));
			TestNotNull(TEXT("Direction"), C->FindPropertyByName(TEXT("Direction")));
			TestNotNull(TEXT("bIsInAir"), C->FindPropertyByName(TEXT("bIsInAir")));
		});

		It(TEXT("P80 Notify 是 UAnimNotify 子类"), [this]()
		{
			TestTrue(TEXT("Notify 子类"),
				UP80_AnimNotify_SendEvent::StaticClass()->IsChildOf(UAnimNotify::StaticClass()));
		});

		It(TEXT("P80 NotifyState 是 UAnimNotifyState 子类"), [this]()
		{
			TestTrue(TEXT("NotifyState 子类"),
				UP80_AnimNotifyState_TrailEffect::StaticClass()->IsChildOf(UAnimNotifyState::StaticClass()));
		});

		It(TEXT("P83 ControlRigHelper 是 ActorComponent"), [this]()
		{
			TestTrue(TEXT("继承 UActorComponent"),
				UP83_ControlRigHelper::StaticClass()->IsChildOf(UActorComponent::StaticClass()));
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
