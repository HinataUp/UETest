#include "P37_LifecyclePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "../00_Common/FundP2LogMacros.h"

AP37_PlayerState::AP37_PlayerState()
{
	bReplicates = true;
}

void AP37_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AP37_PlayerState, PlayerScore);
}

void AP37_PlayerState::OnRep_PlayerScore()
{
	P2_LOG("P37: OnRep PlayerScore = %d", PlayerScore);
}

AP37_PlayerController::AP37_PlayerController()
{
	bReplicates = true;
}

void AP37_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	P2_LOG("P37: PlayerController::BeginPlay LocalRole=%s",
		HasAuthority() ? TEXT("Authority") : TEXT("Autonomous/None"));
}

void AP37_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	P2_LOG("P37: OnPossess %s", InPawn ? *InPawn->GetName() : TEXT("null"));
}

void AP37_PlayerController::OnUnPossess()
{
	P2_LOG("P37: OnUnPossess");
	Super::OnUnPossess();
}
