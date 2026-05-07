// T27 - "Server 权威"模式的最小闭环
//
// 任务目标：跑通 输入 -> Server RPC -> 状态修改 -> 复制回 Client 的标准链路
//
// 这是所有多人战斗的骨架。手感不是最佳（有 RTT 延迟），但绝对安全 —— 数据始终由 Server 决定。
//
// 三种"血量同步"实现对比：
//   写法1（本 task）：Replicated + OnRep        —— 标准状态同步
//   写法2：Multicast RPC 直接广播变化            —— 仅适合"不能用属性同步"的事件
//   写法3：FastArraySerializer                    —— 高频结构数组优化（GAS 用）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "T27_ServerAuthorityCharacter.generated.h"

UCLASS()
class UETEST_API AT27_ServerAuthorityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AT27_ServerAuthorityCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 标准 Server RPC 三件套：UFUNCTION + WithValidation + _Implementation
	// 声明放 public ，便于外部 / 测试直接调用 _Implementation 与 _Validate
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestTakeDamage(int32 T27_Amount);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T27_Health, VisibleAnywhere)
	int32 T27_Health;

	UFUNCTION()
	void OnRep_T27_Health(int32 T27_OldHealth);

	// Client 按 F 时本地调用 -> 内部转发到 ServerRequestTakeDamage
	void T27_OnPressF();
};
