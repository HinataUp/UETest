#include "G35_TagEventListener.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UTagEventListenerComponent::UTagEventListenerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTagEventListenerComponent::BindToASC(UAbilitySystemComponent* ASC)
{
	if (!ASC) { return; }

	BoundASC = ASC;
	BoundHandle = ASC->RegisterGameplayTagEvent(
			MyGameplayTags::TAG_State_Stunned,
			EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &UTagEventListenerComponent::OnStunnedChanged);

	GAS_LOG("【Tag监听】已绑定 State.Stunned 增减事件");
}

void UTagEventListenerComponent::OnStunnedChanged(const FGameplayTag Tag, int32 NewCount)
{
	++CallbackCount;
	if (NewCount > 0)
	{
		GAS_LOG("【Tag监听】Stunned ON ；禁用输入 / 播眩晕动画");
	}
	else
	{
		GAS_LOG("【Tag监听】Stunned OFF ；恢复输入");
	}
}

void UTagEventListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 标准做法：组件销毁时解绑
	if (BoundASC.IsValid() && BoundHandle.IsValid())
	{
		BoundASC->RegisterGameplayTagEvent(
				MyGameplayTags::TAG_State_Stunned,
				EGameplayTagEventType::NewOrRemoved)
			.Remove(BoundHandle);
	}
	Super::EndPlay(EndPlayReason);
}
