// E31 - 自定义 AbilityTask 模板：蓄力释放
//
// 自定义 Task 完整模板：
//   * 静态工厂 NewAbilityTask<T>
//   * Activate 启动逻辑
//   * TickTask 每帧累加（bTickingTask=true）
//   * OnDestroy 清理
//   * ShouldBroadcastAbilityTaskDelegates 检查 OwningAbility 仍有效

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "E31_AbilityTask_WaitChargeRelease.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeReleaseDelegate, float, ChargeDuration);

UCLASS()
class UETEST_API UAbilityTask_WaitChargeRelease : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_WaitChargeRelease(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FChargeReleaseDelegate OnFullyCharged;

	UPROPERTY(BlueprintAssignable)
	FChargeReleaseDelegate OnReleased;

	// 静态工厂：BP 调用入口（即使 GA 是 C++ ，工厂签名也固定如此）
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "WaitChargeRelease",
			HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitChargeRelease* WaitChargeRelease(
		UGameplayAbility* OwningAbility,
		float MaxChargeTime = 2.0f);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bAbilityEnded) override;

	// 由外部调用通知"按键松开"
	void NotifyReleased();

protected:
	float ChargeTime = 0.f;
	float MaxCharge = 2.0f;
	bool  bFullyChargedBroadcast = false;
};
