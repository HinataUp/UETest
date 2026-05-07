// C16 - 三种 Instancing Policy 行为对比
//
// InstancedPerActor    ：99% 场景；每个 Actor 一个实例，状态可累积
// InstancedPerExecution：每次激活 new 一个；完全隔离，但 GC 成本高
// NonInstanced         ：CDO 共享；this 是 CDO ，不能存状态、不能绑 Task（反模式）

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C16_InstancingPolicy.generated.h"

UCLASS()
class UETEST_API UGA_PerActor : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_PerActor();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	// 实例字段：仅 InstancedPerActor / InstancedPerExecution 能保存
	int32 Counter = 0;
};

UCLASS()
class UETEST_API UGA_PerExecution : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_PerExecution();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	int32 Counter = 0;
};
