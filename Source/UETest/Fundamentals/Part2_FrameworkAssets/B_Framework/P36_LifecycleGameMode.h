// P36 - GameMode + GameState
//
// AGameModeBase ：Server Only ：管理规则 / Spawn / Match 流程
//   * Client 端调 GetAuthGameMode() 返回 nullptr （不复制）
// AGameStateBase ：Replicated ：全局共享状态（比赛剩余时间 / 比分）
//   * 所有端可见 ；Server 写入 ，Client OnRep

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "P36_LifecycleGameMode.generated.h"

// ───────────── GameState ─────────────
UCLASS()
class UETEST_API AP36_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AP36_GameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_MatchTimer, VisibleAnywhere)
	float MatchTimer = 0.f;

	UFUNCTION()
	void OnRep_MatchTimer();
};

// ───────────── GameMode ─────────────
UCLASS()
class UETEST_API AP36_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP36_GameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;
};
