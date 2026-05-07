// H41 - 连招状态机 + 取消窗口
// 用 Tag 驱动 Phase ，结合 WaitGameplayEvent 监听窗口

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "H41_ComboGA.generated.h"

UCLASS()
class UETEST_API UGA_ComboAttack : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ComboAttack();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION() void OnComboWindowOpen(FGameplayEventData Payload);
	UFUNCTION() void OnComboWindowClose(FGameplayEventData Payload);

	int32 ComboIndex = 0;

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
	FGameplayAbilityActivationInfo CachedActivationInfo;
};
