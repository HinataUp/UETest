// T23 - NetDormancy 四种状态
//
// 任务目标：让 Server 跳过"长期不变"Actor 的复制扫描
//   * DORM_Awake          ：默认，正常复制
//   * DORM_DormantAll     ：所有连接都休眠
//   * DORM_DormantPartial ：部分连接休眠（按 GetNetDormancy 决定）
//   * DORM_Initial        ：初始即休眠，需要 FlushNetDormancy 才会复制
//
// 经验：地上几百个 Pickup ，默认 DORM_Initial ，玩家拾取时 FlushNetDormancy ，处理完再睡。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T23_NetDormancyActor.generated.h"

UCLASS()
class UETEST_API AT23_NetDormancyActor : public AActor
{
	GENERATED_BODY()

public:
	AT23_NetDormancyActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 T23_RepValue;

private:
	FTimerHandle T23_TickTimer;
	FTimerHandle T23_WakeTimer;
	void T23_ServerTickValue();
	void T23_ServerFlushOnce();
};
