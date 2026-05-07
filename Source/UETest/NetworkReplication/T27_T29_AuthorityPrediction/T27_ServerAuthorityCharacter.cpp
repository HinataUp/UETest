// T27 - Server 权威最小闭环 实现

#include "T27_ServerAuthorityCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "../Common/NetTestLogMacros.h"

AT27_ServerAuthorityCharacter::AT27_ServerAuthorityCharacter()
	: T27_Health(100)
{
	bReplicates = true;
}

void AT27_ServerAuthorityCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT27_ServerAuthorityCharacter, T27_Health);
}

void AT27_ServerAuthorityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 本示例为兼容老 InputComponent ，用 BindKey ；正式工程建议 Enhanced Input
	PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this,
		&AT27_ServerAuthorityCharacter::T27_OnPressF);
}

void AT27_ServerAuthorityCharacter::T27_OnPressF()
{
	// Client 端按下 F ：发 RPC 给 Server，请求扣 10 血
	// 注意：这里不本地修改 ，等 Server 复制回来 —— 这就是"无预测的权威模式"
	NET_LOG(TEXT("Pressed F => ServerRequestTakeDamage(10)"));
	ServerRequestTakeDamage(10);
}

bool AT27_ServerAuthorityCharacter::ServerRequestTakeDamage_Validate(int32 T27_Amount)
{
	return T27_Amount > 0 && T27_Amount <= 1000;
}

void AT27_ServerAuthorityCharacter::ServerRequestTakeDamage_Implementation(int32 T27_Amount)
{
	// 业务规则在这里执行；非法情况温和返回，不要 _Validate=false
	if (T27_Health <= 0) { return; }

	const int32 OldHealth = T27_Health;
	T27_Health = FMath::Max(0, T27_Health - T27_Amount);
	NET_LOG(TEXT("ServerExec Damage=%d Health %d -> %d"),
		T27_Amount, OldHealth, T27_Health);
}

void AT27_ServerAuthorityCharacter::OnRep_T27_Health(int32 T27_OldHealth)
{
	// 此处可以播受伤反馈 / 更新血条 UI
	NET_LOG(TEXT("OnRep_Health Old=%d New=%d"), T27_OldHealth, T27_Health);
}
