// T20 - Server RPC 的 Validation
//
// 任务目标：保护 Server 不被恶意 Client 攻击
//   * _Validate 返回 false 会 触发 RPC_ValidateFailed ，默认行为是断开该 Client 连接
//   * 这是严肃后果，不要拿来做"业务规则不通过"的判断
//
// 经验法则：
//   * _Validate 只检查"明显作弊"的越界（数值范围、字符串长度）
//   * 业务规则（冷却、资源、距离）放在 _Implementation 里温和拒绝即可
//   * 所有 Server RPC 应该 WithValidation ，是工程安全基线

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "T20_RPCValidationPawn.generated.h"

UCLASS()
class UETEST_API AT20_RPCValidationPawn : public APawn
{
	GENERATED_BODY()

public:
	AT20_RPCValidationPawn();
	virtual void BeginPlay() override;

	// 写法：UFUNCTION(Server, Reliable, WithValidation)
	// UHT 会要求你额外实现 _Validate ；返回 bool
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSafeAction(int32 T20_Damage);

private:
	FTimerHandle T20_TickTimer;
	int32 T20_TickCount = 0;
	void T20_ClientFireAction();
};
