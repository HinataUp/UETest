// G35 - RegisterGameplayTagEvent 监听 Tag 增减
// 用 Component 或 Actor 持有监听句柄 ，便于解绑

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "G35_TagEventListener.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup = (GAS), meta = (BlueprintSpawnableComponent))
class UETEST_API UTagEventListenerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UTagEventListenerComponent();

	// 业务接口：调一次绑定到目标 ASC
	void BindToASC(UAbilitySystemComponent* ASC);

	int32 GetCallbackCount() const { return CallbackCount; }

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnStunnedChanged(const FGameplayTag Tag, int32 NewCount);

private:
	TWeakObjectPtr<UAbilitySystemComponent> BoundASC;
	FDelegateHandle BoundHandle;
	int32 CallbackCount = 0;
};
