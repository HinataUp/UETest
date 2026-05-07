#include "P06_FunctionSpecifierActor.h"
#include "../00_Common/FundamentalsLogMacros.h"

AP06_FunctionSpecifierPawn::AP06_FunctionSpecifierPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AP06_FunctionSpecifierPawn::DoAction()
{
	P1_LOG("P06: DoAction 调用");
}

bool AP06_FunctionSpecifierPawn::ServerSendInput_Validate(int32 InputCode)
{
	// 范围校验 ：拦截明显作弊 ；返回 false 会断开连接 ，慎用
	return InputCode >= 0 && InputCode <= 1000;
}

void AP06_FunctionSpecifierPawn::ServerSendInput_Implementation(int32 InputCode)
{
	P1_LOG("P06: Server 收到输入 %d", InputCode);
}

void AP06_FunctionSpecifierPawn::DebugRespawn()
{
	P1_LOG("P06: 控制台 Exec DebugRespawn 触发");
}
