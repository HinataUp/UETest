// T6 - RepNotify 基础：无参 OnRep 回调
//
// 任务目标：让 Client 在收到属性复制时自动得到通知，而不是轮询
// 验收：
//   * Authority   端默认不调用 OnRep
//   * Simulated  端必调用 OnRep
//
// 几种 OnRep 命名 / 写法：
//   写法1：UFUNCTION() void OnRep_RepInt();                    （本 task ）
//   写法2：UFUNCTION() void OnRep_RepInt(int32 OldValue);      （T7）
//   写法3：UFUNCTION() void OnRep_RepInt(const FStruct& Old);  （结构体的旧值）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T6_OnRepBasicActor.generated.h"

UCLASS()
class UETEST_API AT6_OnRepBasicActor : public AActor
{
	GENERATED_BODY()

public:
	AT6_OnRepBasicActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// ReplicatedUsing= 指定 OnRep 函数名
	UPROPERTY(ReplicatedUsing = OnRep_T6_RepInt, VisibleAnywhere)
	int32 T6_RepInt;

	// OnRep 必须加 UFUNCTION() ，否则反射系统找不到，不会被回调
	UFUNCTION()
	void OnRep_T6_RepInt();

private:
	FTimerHandle T6_TickTimer;
	void T6_ServerTickValue();
};
