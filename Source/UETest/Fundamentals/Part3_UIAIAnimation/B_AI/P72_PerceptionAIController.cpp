#include "P72_PerceptionAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "../00_Common/FundP3LogMacros.h"

AP72_PerceptionAIController::AP72_PerceptionAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	// 视觉配置 ：半径 / 失去视野半径 / 检测哪些阵营
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 2000.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());
}

void AP72_PerceptionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (PerceptionComp)
	{
		// AddDynamic ：BP 友好的 UFUNCTION 路径
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(
			this, &AP72_PerceptionAIController::HandleTargetPerceptionUpdated);
	}
}

void AP72_PerceptionAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Stimulus.WasSuccessfullySensed() ：true 表示"感知到"，false 表示"丢失"
	P3_LOG("P72: Perception %s %s",
		Actor ? *Actor->GetName() : TEXT("null"),
		Stimulus.WasSuccessfullySensed() ? TEXT("Sensed") : TEXT("Lost"));
}
