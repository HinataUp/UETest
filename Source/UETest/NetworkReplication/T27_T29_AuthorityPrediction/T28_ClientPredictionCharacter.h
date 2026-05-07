// T28 - 客户端预测最简形态
//
// 任务目标：在 T27 基础上加上"乐观预测"
//   * Client 按 F 时本地立刻扣血
//   * 同时发 ServerRequestTakeDamage
//   * Server 处理结果通过 OnRep 回到 Client
//   * 若 Server 拒绝（如 Health<50 不准扣），OnRep 会把 Health 拉回到权威值
//
// 注意：这是教学版预测，没有"输入序号 / 历史回放"。GAS 的 Prediction 是同思想的工业级实现。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "T28_ClientPredictionCharacter.generated.h"

UCLASS()
class UETEST_API AT28_ClientPredictionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AT28_ClientPredictionCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T28_Health, VisibleAnywhere)
	int32 T28_Health;

	UFUNCTION()
	void OnRep_T28_Health(int32 T28_OldHealth);

	void T28_OnPressF();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestTakeDamage(int32 T28_Amount);
};
