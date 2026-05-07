#include "E31_AbilityTask_WaitChargeRelease.h"
#include "../00_Common/GASLogMacros.h"

UAbilityTask_WaitChargeRelease::UAbilityTask_WaitChargeRelease(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_WaitChargeRelease* UAbilityTask_WaitChargeRelease::WaitChargeRelease(
	UGameplayAbility* OwningAbility, float MaxChargeTime)
{
	UAbilityTask_WaitChargeRelease* Task = NewAbilityTask<UAbilityTask_WaitChargeRelease>(OwningAbility);
	Task->MaxCharge = MaxChargeTime;
	return Task;
}

void UAbilityTask_WaitChargeRelease::Activate()
{
	GAS_LOG("【WaitCharge】启动 ，MaxCharge=%.2f", MaxCharge);
}

void UAbilityTask_WaitChargeRelease::TickTask(float DeltaTime)
{
	ChargeTime += DeltaTime;

	if (!bFullyChargedBroadcast && ChargeTime >= MaxCharge)
	{
		bFullyChargedBroadcast = true;
		// ShouldBroadcastAbilityTaskDelegates 检查 GA 是否仍然激活
		// 防止 GA 已被 Cancel 但 Task 还没来得及清理
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			GAS_LOG("【WaitCharge】满蓄力");
			OnFullyCharged.Broadcast(ChargeTime);
		}
	}
}

void UAbilityTask_WaitChargeRelease::NotifyReleased()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		const float Duration = FMath::Min(ChargeTime, MaxCharge);
		GAS_LOG("【WaitCharge】Released Duration=%.2f", Duration);
		OnReleased.Broadcast(Duration);
	}
	EndTask();
}

void UAbilityTask_WaitChargeRelease::OnDestroy(bool bAbilityEnded)
{
	GAS_LOG("【WaitCharge】OnDestroy bAbilityEnded=%d", bAbilityEnded ? 1 : 0);
	Super::OnDestroy(bAbilityEnded);
}
