// P72 - AI Perception ：视觉 / 听觉 / 伤害 感知
//
// 用法 ：在 AIController 上挂 UAIPerceptionComponent ；配 SenseConfig
// OnTargetPerceptionUpdated 回调 ：感知到 / 失去目标都触发

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P72_PerceptionAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
struct FAIStimulus;

UCLASS()
class UETEST_API AP72_PerceptionAIController : public AAIController
{
	GENERATED_BODY()

public:
	AP72_PerceptionAIController();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
