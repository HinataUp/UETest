// T15 - PreReplication 动态条件
//
// 任务目标：当静态 COND_xxx 不够用时，运行时动态决定字段要不要复制
// 典型场景：
//   * "战斗中"才同步实时数据，"大厅"里不浪费带宽
//   * 角色处于隐身状态时不暴露位置 / 朝向
//
// 注意：PreReplication 每次 Server 复制本 Actor 之前都会调用，不要做重计算。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T15_PreReplicationActor.generated.h"

UCLASS()
class UETEST_API AT15_PreReplicationActor : public AActor
{
	GENERATED_BODY()

public:
	AT15_PreReplicationActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 关键：override PreReplication ，调用 DOREPLIFETIME_ACTIVE_OVERRIDE_FAST
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 T15_ConditionalField;

	// 控制开关：true 时该字段才同步
	UPROPERTY(VisibleAnywhere)
	bool T15_bShouldReplicate;

private:
	FTimerHandle T15_TickValueTimer;
	FTimerHandle T15_ToggleTimer;
	void T15_ServerTickValue();
	void T15_ServerToggleSwitch();
};
