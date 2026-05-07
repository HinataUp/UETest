#include "E30_GA_TargetData.h"
#include "../00_Common/GASLogMacros.h"

UGA_TargetData::UGA_TargetData()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_TargetData::ActivateAbility(
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

	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;

	// 真实项目：用 UAbilityTask_WaitTargetData::WaitTargetData(...) +
	// AGameplayAbilityTargetActor_SingleLineTrace 类
	// 本示例不引入 TargetActor 子类（涉及碰撞 / 输入耦合），
	// 仅演示回调签名与 EndAbility 时机
	GAS_LOG("【TargetData】GA 启动，等待目标确认（实际运行需配 TargetActor 类）");

	// 模拟即时确认：直接 EndAbility（占位）
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_TargetData::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	GAS_LOG("【TargetData】ValidData ，DataNum=%d", Data.Num());
	// 取出 HitResult ，对目标 ApplyGameplayEffect
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}

void UGA_TargetData::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	GAS_LOG("【TargetData】Cancelled");
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, true);
}
