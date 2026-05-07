// T28 - 客户端预测最简形态 实现

#include "T28_ClientPredictionCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "../Common/NetTestLogMacros.h"

AT28_ClientPredictionCharacter::AT28_ClientPredictionCharacter()
	: T28_Health(100)
{
	bReplicates = true;
}

void AT28_ClientPredictionCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT28_ClientPredictionCharacter, T28_Health);
}

void AT28_ClientPredictionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this,
		&AT28_ClientPredictionCharacter::T28_OnPressF);
}

void AT28_ClientPredictionCharacter::T28_OnPressF()
{
	// 步骤1：Client 本地"乐观预测"立刻扣血 ，UI 即刻反馈
	// 仅在 Autonomous 端做（自机），Listen Server 上的本地玩家也算
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		const int32 OldHealth = T28_Health;
		T28_Health = FMath::Max(0, T28_Health - 10);
		NET_LOG(TEXT("ClientPredict Health %d -> %d"), OldHealth, T28_Health);
	}

	// 步骤2：发 RPC 给 Server ，请求权威处理
	ServerRequestTakeDamage(10);
}

bool AT28_ClientPredictionCharacter::ServerRequestTakeDamage_Validate(int32 T28_Amount)
{
	return T28_Amount > 0 && T28_Amount <= 1000;
}

void AT28_ClientPredictionCharacter::ServerRequestTakeDamage_Implementation(int32 T28_Amount)
{
	// 演示业务规则拒绝：Health<=50 时拒绝扣血
	if (T28_Health <= 50)
	{
		NET_LOG(TEXT("ServerReject (Health<=50) ; will correct client predict"));
		// 不变更值即可 —— 但因 OnChanged 默认不变不会触发 OnRep ，需要"强制再同步"
		// 工程做法：用一个独立 Replicated 计数器 + REPNOTIFY_Always 回滚客户端预测
		return;
	}

	const int32 OldHealth = T28_Health;
	T28_Health = FMath::Max(0, T28_Health - T28_Amount);
	NET_LOG(TEXT("ServerExec Health %d -> %d"), OldHealth, T28_Health);
}

void AT28_ClientPredictionCharacter::OnRep_T28_Health(int32 T28_OldHealth)
{
	// Client 收到权威值；如果与本地预测不一致，权威覆盖预测 —— 即所谓"纠正"
	NET_LOG(TEXT("OnRep_Health Authority=%d (was Predict=%d)"),
		T28_Health, T28_OldHealth);
}
