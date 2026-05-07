// P59 - 网络复制基础（综合示例）
//
// 注意 ：与 NetworkReplication 模块的 30 个细分 task 不重叠 ；这里只综合演示 一个 Actor 用法

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P59_NetworkBasicsActor.generated.h"

UCLASS()
class UETEST_API AP59_NetworkBasicsActor : public AActor
{
	GENERATED_BODY()

public:
	AP59_NetworkBasicsActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Replicated + OnRep
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere)
	int32 Health = 100;

	UFUNCTION()
	void OnRep_Health();

	// Server RPC
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestDamage(int32 Amount);

	// Client RPC
	UFUNCTION(Client, Reliable)
	void ClientShowMessage(const FString& Msg);

	// Multicast
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayHitFX(FVector Location);
};
