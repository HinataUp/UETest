// T25 - Component 复制
//
// 任务目标：理解两个独立开关
//   * Actor.bReplicates             ：Actor 参与复制
//   * UActorComponent.SetIsReplicatedByDefault / SetIsReplicated
//   只有两个都开，Component 上的 Replicated 字段才会同步
//
// UE5 推荐 SetIsReplicatedByDefault(true) 在构造期声明，等价于 BP 中"Component Replicates"勾选

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T25_RepCombatComponent.generated.h"

UCLASS(ClassGroup = (Networking), meta = (BlueprintSpawnableComponent))
class UETEST_API UT25_RepCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT25_RepCombatComponent();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_T25_CompValue, VisibleAnywhere)
	int32 T25_CompValue;

	UFUNCTION()
	void OnRep_T25_CompValue();

private:
	FTimerHandle T25_TickTimer;
	void T25_ServerTick();
};

// 一个用来挂载该 Component 的 Actor
UCLASS()
class UETEST_API AT25_RepComponentActor : public AActor
{
	GENERATED_BODY()
public:
	AT25_RepComponentActor();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UT25_RepCombatComponent> T25_CombatComp;
};
