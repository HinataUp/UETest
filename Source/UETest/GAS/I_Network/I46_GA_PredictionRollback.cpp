#include "I46_GA_PredictionRollback.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "../00_Common/GASLogMacros.h"

UGA_PredictionRollback::UGA_PredictionRollback()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGA_PredictionRollback::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bSuper = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bSuper) { return false; }

	// 关键 ：Server 端 50% 概率拒绝；Client 端始终通过
	const AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	const bool bServer = Avatar && Avatar->HasAuthority();
	if (bServer && FMath::FRand() < 0.5f)
	{
		GAS_LOG("【预测回滚】Server 拒绝激活 ；Client 已做的预测改动会被回滚");
		return false;
	}

	return true;
}

void UGA_PredictionRollback::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData*)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 关键 ：用 ApplyGameplayEffectToOwner 应用一个 Instant GE 扣血
	// LocalPredicted 模式下 GAS 自动用 PredictionKey 关联此次改动
	GAS_LOG("【预测回滚】Activate 成功，应用预测式 GE （此处省略 GE）");

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
