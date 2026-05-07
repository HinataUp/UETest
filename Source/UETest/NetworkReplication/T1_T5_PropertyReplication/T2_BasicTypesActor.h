// T2 - 基础数据类型复制（int / float / bool / FString / FName）
//
// 任务目标：把最常用的标量类型都复制一遍，建立"按值同步"的心理模型
// 关键观察：FString 体积明显大于 int / FName，频繁复制长字符串是典型反模式
//
// 常见写法（DOREPLIFETIME 三种）：
//   1) DOREPLIFETIME(Class, Field)                          ：默认条件
//   2) DOREPLIFETIME_CONDITION(Class, Field, COND_xxx)      ：带条件，见 T14
//   3) DOREPLIFETIME_WITH_PARAMS_FAST(...)                  ：UE5 优化版，性能敏感时使用

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T2_BasicTypesActor.generated.h"

UCLASS()
class UETEST_API AT2_BasicTypesActor : public AActor
{
	GENERATED_BODY()

public:
	AT2_BasicTypesActor();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere) int32   T2_RepInt;
	UPROPERTY(Replicated, VisibleAnywhere) float   T2_RepFloat;
	UPROPERTY(Replicated, VisibleAnywhere) bool    T2_bRepBool;
	UPROPERTY(Replicated, VisibleAnywhere) FString T2_RepString;
	UPROPERTY(Replicated, VisibleAnywhere) FName   T2_RepName;

private:
	FTimerHandle T2_TickTimer;
	void T2_ServerTickAll();
};
