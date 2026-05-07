// P80 - AnimNotifyState ：持续状态 Notify
//
// NotifyBegin ：进入区间触发
// NotifyTick  ：区间内每帧
// NotifyEnd   ：区间结束（自然结束 / 中断）触发
//
// 实战 ：拖尾粒子 ：Begin Spawn + End Destroy

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "P80_AnimNotifyState_TrailEffect.generated.h"

UCLASS(meta = (DisplayName = "P80 Trail Effect (Notify State)"))
class UETEST_API UP80_AnimNotifyState_TrailEffect : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
};
