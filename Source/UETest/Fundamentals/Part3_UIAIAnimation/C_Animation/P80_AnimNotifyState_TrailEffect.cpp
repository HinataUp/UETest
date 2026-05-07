#include "P80_AnimNotifyState_TrailEffect.h"
#include "Components/SkeletalMeshComponent.h"
#include "../00_Common/FundP3LogMacros.h"

void UP80_AnimNotifyState_TrailEffect::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	P3_LOG("P80: NotifyState Begin（实战 ：Spawn 拖尾 NiagaraSystem）");
	// 实战 ：UNiagaraFunctionLibrary::SpawnSystemAttached(...)
}

void UP80_AnimNotifyState_TrailEffect::NotifyTick(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	// 实战 ：每帧更新拖尾末端跟踪武器位置
}

void UP80_AnimNotifyState_TrailEffect::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	P3_LOG("P80: NotifyState End（销毁拖尾）");
}
