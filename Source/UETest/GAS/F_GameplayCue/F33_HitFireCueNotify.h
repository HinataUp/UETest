// F33 - Burst Cue（一次性）
// Static 型 Cue ，OnExecute 触发 ；典型用途：命中爆炸、击中粒子
//
// Tag 命名必须以 GameplayCue. 开头，CueManager 才会扫描

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "F33_HitFireCueNotify.generated.h"

UCLASS()
class UETEST_API UCueNotify_HitFire : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UCueNotify_HitFire();
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
