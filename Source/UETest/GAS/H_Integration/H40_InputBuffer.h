// H40 - 输入缓冲：Tag 驱动的 Pending Input
// ACT 手感核心：当前 GA 阻塞输入时 Push ；窗口一开 Consume

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "H40_InputBuffer.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup = (GAS), meta = (BlueprintSpawnableComponent))
class UETEST_API UInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInputBufferComponent();

	// 推入一条待消费输入
	UFUNCTION(BlueprintCallable)
	void PushBufferedInput(FGameplayTag InputTag, float WindowSeconds = 0.3f);

	// 消费 ：检查 Tag 存在 -> 清除 + 返回 true
	UFUNCTION(BlueprintCallable)
	bool ConsumeBufferedInput(FGameplayTag InputTag);

	void SetTargetASC(UAbilitySystemComponent* InASC) { ASC = InASC; }

private:
	void OnBufferTimeout(FGameplayTag InputTag);

	UPROPERTY() TObjectPtr<UAbilitySystemComponent> ASC;
	TMap<FGameplayTag, FTimerHandle> PendingTimers;
};
