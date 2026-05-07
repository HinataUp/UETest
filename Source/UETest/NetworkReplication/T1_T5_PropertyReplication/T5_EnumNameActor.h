// T5 - 枚举 / FName 等"低成本类型"复制
//
// 任务目标：
//   * 枚举：体积极小，是同步状态机的首选
//   * FName ：底层是 NameTable 索引，复制时只传 ID + 必要时同步字符串
//
// 注意：UENUM 必须显式 ": uint8" 才能参与反射 / 蓝图 / 复制；
//       这是 UE 的硬性要求，而不是风格建议。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T5_EnumNameActor.generated.h"

UENUM(BlueprintType)
enum class ET5_CombatPhase : uint8
{
	Idle      UMETA(DisplayName = "Idle"),
	Attacking UMETA(DisplayName = "Attacking"),
	Blocking  UMETA(DisplayName = "Blocking"),
	Stunned   UMETA(DisplayName = "Stunned"),
};

UCLASS()
class UETEST_API AT5_EnumNameActor : public AActor
{
	GENERATED_BODY()

public:
	AT5_EnumNameActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere)
	ET5_CombatPhase T5_Phase;

	// 用于演示：FName 复制极快 —— 适合"状态名 / 行为名"等小集合
	UPROPERTY(Replicated, VisibleAnywhere)
	FName T5_StateName;

private:
	FTimerHandle T5_TickTimer;
	int32 T5_TickCount = 0;
	void T5_ServerTickPhase();
};
