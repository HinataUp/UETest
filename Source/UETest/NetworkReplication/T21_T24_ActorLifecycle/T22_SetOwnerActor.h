// T22 - SetOwner 对 Autonomous 判定的影响
//
// 任务目标：理解"Autonomous Proxy"身份从何而来
//   规则：Actor.Owner 链 -> PlayerController ， 与 Local Connection 相同 => 该端为 Autonomous
//
// 关键点：
//   * SetOwner 只能在 Server 调用
//   * Owner 通常是 PlayerController 或 Pawn
//   * 玩家自己的角色由 Possess 自动建立 Owner 链；场景 Actor 需要手动 SetOwner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T22_SetOwnerActor.generated.h"

UCLASS()
class UETEST_API AT22_SetOwnerActor : public AActor
{
	GENERATED_BODY()

public:
	AT22_SetOwnerActor();
	virtual void BeginPlay() override;

private:
	FTimerHandle T22_AssignTimer;
	FTimerHandle T22_LogTimer;
	void T22_ServerAssignOwner();
	void T22_LogRoleEachSecond();
};
