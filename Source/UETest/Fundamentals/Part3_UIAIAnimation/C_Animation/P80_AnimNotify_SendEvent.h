// P80 - 自定义 AnimNotify ：在动画特定帧发送 GameplayEvent
//
// 用法 ：在 Anim Sequence / Montage Timeline 的某帧上加一个 P80 Notify ；播放到该帧时 Notify::Notify 触发
// 实战 ：连招攻击的"命中帧"通过 EventTag 通知 GA / 战斗系统

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "P80_AnimNotify_SendEvent.generated.h"

UCLASS(meta = (DisplayName = "P80 Send Gameplay Event"))
class UETEST_API UP80_AnimNotify_SendEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "P80")
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, Category = "P80")
	float EventMagnitude = 0.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	// 编辑器中显示的名字
	virtual FString GetNotifyName_Implementation() const override;
};
