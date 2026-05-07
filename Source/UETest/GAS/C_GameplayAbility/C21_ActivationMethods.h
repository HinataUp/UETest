// C21 - GA 五种激活方式（演示用 GA + Trigger 配置）
//
// 1. ByClass    ：TryActivateAbilityByClass(Class)
// 2. ByTag      ：TryActivateAbilitiesByTag(Container)
// 3. ByHandle   ：TryActivateAbility(Handle)
// 4. ByEvent    ：SendGameplayEventToActor(Tag, EventData) —— GA 需配 AbilityTriggers
// 5. OneShot    ：GiveAbilityAndActivateOnce(Spec) —— 用完即清

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C21_ActivationMethods.generated.h"

// 配 AbilityTriggers ，能被 Event 触发
UCLASS()
class UETEST_API UGA_TriggerByEvent : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_TriggerByEvent();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};

// OneShot：与 GiveAbilityAndActivateOnce 配合，结束后自动 ClearAbility
UCLASS()
class UETEST_API UGA_OneShot : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_OneShot();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
