// F33 - Actor 型 Cue（持续性）
// 适合燃烧、光环等持续视觉表现
// 四个事件 ：OnActive / WhileActive / OnRemove / OnExecute

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "F33_BurningCueNotify.generated.h"

UCLASS()
class UETEST_API ACueNotify_Burning : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	ACueNotify_Burning();

	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};
