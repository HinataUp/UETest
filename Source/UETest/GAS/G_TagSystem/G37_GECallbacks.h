// G37 - GE 生命周期回调 + Attribute Value Change Delegate
//
// 三个 ASC 委托 ：
//   OnGameplayEffectAppliedDelegateToSelf   ：任何 GE 应用都触发（含 Instant）
//   OnActiveGameplayEffectAddedDelegateToSelf：仅 Duration/Infinite 进入 Active 时
//   OnAnyGameplayEffectRemovedDelegate      ：任意 ActiveGE 被移除（到期或主动）
//
// 一个属性委托 ：
//   GetGameplayAttributeValueChangeDelegate ：属性变化时触发；UI 绑定首选

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "G37_GECallbacks.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
struct FGameplayEffectSpec;
struct FActiveGameplayEffect;
struct FOnAttributeChangeData;

UCLASS(ClassGroup = (GAS), meta = (BlueprintSpawnableComponent))
class UETEST_API UGECallbacksComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UGECallbacksComponent();
	void BindToASC(UAbilitySystemComponent* ASC);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnAnyGEApplied(UAbilitySystemComponent* Source,
		const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void OnGEAdded(UAbilitySystemComponent* Source,
		const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void OnGERemoved(const FGameplayEffectRemovalInfo& Info);
	void OnHealthChanged(const FOnAttributeChangeData& Data);

private:
	TWeakObjectPtr<UAbilitySystemComponent> BoundASC;
	FDelegateHandle AppliedHandle, AddedHandle, RemovedHandle, AttrHandle;
};
