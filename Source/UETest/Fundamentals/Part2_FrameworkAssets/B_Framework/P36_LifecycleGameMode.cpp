#include "P36_LifecycleGameMode.h"
#include "Net/UnrealNetwork.h"
#include "../00_Common/FundP2LogMacros.h"

// ───────────────── GameState ─────────────────

AP36_GameState::AP36_GameState()
{
	bReplicates = true;
}

void AP36_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AP36_GameState, MatchTimer);
}

void AP36_GameState::OnRep_MatchTimer()
{
	P2_LOG("P36: OnRep MatchTimer = %.2f （Client 收到同步）", MatchTimer);
}

// ───────────────── GameMode ─────────────────

AP36_GameMode::AP36_GameMode()
{
	GameStateClass = AP36_GameState::StaticClass();
}

void AP36_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	P2_LOG("P36: GameMode::InitGame Map=%s", *MapName);
}

void AP36_GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	P2_LOG("P36: GameMode::HandleStartingNewPlayer");
}

void AP36_GameMode::StartPlay()
{
	Super::StartPlay();
	P2_LOG("P36: GameMode::StartPlay");
}
