#include "H40_InputBuffer.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "../00_Common/GASLogMacros.h"

UInputBufferComponent::UInputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInputBufferComponent::PushBufferedInput(FGameplayTag InputTag, float WindowSeconds)
{
	if (!ASC) { return; }

	// 用 LooseTag 标记 ；Loose 不复制 ，本地判定
	ASC->AddLooseGameplayTag(InputTag);
	GAS_LOG("【输入缓冲】Push %s（窗口 %.2fs）", *InputTag.ToString(), WindowSeconds);

	// 启动 Timer ：到时未消费则清除
	FTimerHandle& Handle = PendingTimers.FindOrAdd(InputTag);
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateUObject(this, &UInputBufferComponent::OnBufferTimeout, InputTag),
			WindowSeconds, false);
	}
}

bool UInputBufferComponent::ConsumeBufferedInput(FGameplayTag InputTag)
{
	if (!ASC) { return false; }
	if (!ASC->HasMatchingGameplayTag(InputTag)) { return false; }

	ASC->RemoveLooseGameplayTag(InputTag);
	if (FTimerHandle* H = PendingTimers.Find(InputTag))
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(*H);
		}
		PendingTimers.Remove(InputTag);
	}
	GAS_LOG("【输入缓冲】Consume %s 成功", *InputTag.ToString());
	return true;
}

void UInputBufferComponent::OnBufferTimeout(FGameplayTag InputTag)
{
	if (!ASC) { return; }
	if (ASC->HasMatchingGameplayTag(InputTag))
	{
		ASC->RemoveLooseGameplayTag(InputTag);
		GAS_LOG("【输入缓冲】%s 超时清除", *InputTag.ToString());
	}
	PendingTimers.Remove(InputTag);
}
