#include "C17_NetExecPolicy.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/GASLogMacros.h"

namespace
{
	// 公用日志：打印当前端的 Role + 标签
	void LogActivate(const TCHAR* Label, const FGameplayAbilityActorInfo* ActorInfo)
	{
		const AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
		const TCHAR* RoleStr = TEXT("Unknown");
		if (Avatar)
		{
			switch (Avatar->GetLocalRole())
			{
				case ROLE_Authority:       RoleStr = TEXT("Authority"); break;
				case ROLE_AutonomousProxy: RoleStr = TEXT("Autonomous"); break;
				case ROLE_SimulatedProxy:  RoleStr = TEXT("Simulated"); break;
				default: break;
			}
		}
		GAS_LOG("【NetPolicy】%s 在 %s 端 Activate", Label, RoleStr);
	}
}

UGA_NetLocalOnly::UGA_NetLocalOnly()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}
void UGA_NetLocalOnly::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	LogActivate(TEXT("LocalOnly"), AI);
	EndAbility(H, AI, I, true, false);
}

UGA_NetLocalPredicted::UGA_NetLocalPredicted()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}
void UGA_NetLocalPredicted::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	LogActivate(TEXT("LocalPredicted"), AI);
	EndAbility(H, AI, I, true, false);
}

UGA_NetServerOnly::UGA_NetServerOnly()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}
void UGA_NetServerOnly::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	LogActivate(TEXT("ServerOnly"), AI);
	EndAbility(H, AI, I, true, false);
}

UGA_NetServerInitiated::UGA_NetServerInitiated()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}
void UGA_NetServerInitiated::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	LogActivate(TEXT("ServerInitiated"), AI);
	EndAbility(H, AI, I, true, false);
}
