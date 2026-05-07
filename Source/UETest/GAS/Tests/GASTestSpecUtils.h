// GAS 自动化测试公共辅助
// 提供 NewObject 出 ASC + AttributeSet 的最小测试环境

#pragma once

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../A_Basics/A02_CombatSet.h"
#include "../00_Common/GASTestActor.h"

namespace GASTestSpecUtils
{
	// 构造一个最小化测试环境 ：Outer + ASC + 两个 Set
	struct FTestEnv
	{
		AGASTestActor* Actor = nullptr;
		UAbilitySystemComponent* ASC = nullptr;
		UHealthSet* HealthSet = nullptr;
		UCombatSet* CombatSet = nullptr;

		void Setup()
		{
			Actor = NewObject<AGASTestActor>();
			if (Actor)
			{
				ASC = Actor->GetAbilitySystemComponent();
				HealthSet = Actor->GetHealthSet();
				CombatSet = Actor->GetCombatSet();
			}
		}

		bool IsValid() const { return Actor && ASC && HealthSet && CombatSet; }
	};

	// 在 ASC 上 Apply 一个 GE 类，返回 ActiveHandle
	inline FActiveGameplayEffectHandle ApplyGEToSelf(
		UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> GEClass, float Level = 1.f)
	{
		if (!ASC || !GEClass) { return FActiveGameplayEffectHandle(); }
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GEClass, Level, Ctx);
		if (!Spec.IsValid()) { return FActiveGameplayEffectHandle(); }
		return ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

#endif // WITH_DEV_AUTOMATION_TESTS
