// T11 - 复制 Server 运行时 Spawn 的 Actor 指针
//
// 任务目标：理解"Unmapped Reference"现象
//   Server 同帧 Spawn ActorB 并把 ActorA.Ref = ActorB ；
//   Client 收 ActorA 复制时，ActorB 可能还没到达，导致 OnRep 拿到 nullptr 。
//   UE 会延迟解析（pending GUID），稍后再触发一次 OnRep 。
//
// 工程经验：
//   * OnRep 第一次拿到 null 是常见现象，必须容错
//   * 被引用的 Actor 必须 bReplicates=true ，否则 Client 上根本没有

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T11_SpawnedActorRefActor.generated.h"

UCLASS()
class UETEST_API AT11_SpawnedActorRefActor : public AActor
{
	GENERATED_BODY()

public:
	AT11_SpawnedActorRefActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 蓝图中指定要 Spawn 的 Actor 类（例如指回 ANetTestActor）
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> T11_SpawnClass;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T11_SpawnedRef, VisibleAnywhere)
	TObjectPtr<AActor> T11_SpawnedRef;

	UFUNCTION()
	void OnRep_T11_SpawnedRef();

private:
	FTimerHandle T11_TickTimer;
	void T11_ServerSpawnAndRef();
};
