#include "P80_AnimNotify_SendEvent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../00_Common/FundP3LogMacros.h"

void UP80_AnimNotify_SendEvent::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !EventTag.IsValid()) { return; }
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) { return; }

	// 用 GAS 的 SendGameplayEventToActor ：也可以发到自定义事件总线
	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.EventMagnitude = EventMagnitude;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, EventData);

	P3_LOG("P80: AnimNotify 发送事件 %s ：Magnitude=%.1f",
		*EventTag.ToString(), EventMagnitude);
}

FString UP80_AnimNotify_SendEvent::GetNotifyName_Implementation() const
{
	return EventTag.IsValid()
		? FString::Printf(TEXT("Send: %s"), *EventTag.ToString())
		: TEXT("Send Event (Tag 未配)");
}
