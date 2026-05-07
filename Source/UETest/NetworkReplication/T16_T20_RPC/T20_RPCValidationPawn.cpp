// T20 - Server RPC Validation 实现

#include "T20_RPCValidationPawn.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT20_RPCValidationPawn::AT20_RPCValidationPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT20_RPCValidationPawn::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_AutonomousProxy || (HasAuthority() && IsLocallyControlled()))
	{
		GetWorldTimerManager().SetTimer(
			T20_TickTimer, this, &AT20_RPCValidationPawn::T20_ClientFireAction,
			2.0f, true);
	}
}

void AT20_RPCValidationPawn::T20_ClientFireAction()
{
	// 偶数次合法、奇数次发"非法值"以演示 Validation
	const int32 Damage = (T20_TickCount % 2 == 0) ? 10 : 99999;
	++T20_TickCount;

	NET_LOG(TEXT("ClientFire Damage=%d"), Damage);
	ServerSafeAction(Damage);
}

// _Validate ：纯净的范围检查；返回 false 触发连接断开（生产中谨慎使用）
bool AT20_RPCValidationPawn::ServerSafeAction_Validate(int32 T20_Damage)
{
	const bool bOK = (T20_Damage >= 0 && T20_Damage <= 1000);
	if (!bOK)
	{
		NET_LOG(TEXT("Validate FAILED Damage=%d"), T20_Damage);
	}
	return bOK;
}

void AT20_RPCValidationPawn::ServerSafeAction_Implementation(int32 T20_Damage)
{
	// 业务规则在这里温和处理（如冷却中可直接 return ，不要 _Validate=false）
	NET_LOG(TEXT("ServerExecute Damage=%d"), T20_Damage);
}
