// P37 - PlayerController + PlayerState
//
// PlayerController ：每端一个 ；输入 / UI / Camera / Server 权威
//   * Server 拥有所有 PC ；每个 OwningClient 有自己的 PC ；Simulated Proxy 无 PC
// PlayerState ：Replicated ；玩家数据 ；跨 Pawn 持久（重生时 Pawn 销毁但 PS 不变）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "P37_LifecyclePlayerController.generated.h"

// ───────────── PlayerState ─────────────
UCLASS()
class UETEST_API AP37_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AP37_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerScore, VisibleAnywhere)
	int32 PlayerScore = 0;

	UFUNCTION()
	void OnRep_PlayerScore();
};

// ───────────── PlayerController ─────────────
UCLASS()
class UETEST_API AP37_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AP37_PlayerController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
