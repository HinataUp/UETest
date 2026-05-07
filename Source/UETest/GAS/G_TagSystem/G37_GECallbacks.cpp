#include "G37_GECallbacks.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/GASLogMacros.h"

UGECallbacksComponent::UGECallbacksComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGECallbacksComponent::BindToASC(UAbilitySystemComponent* ASC)
{
	if (!ASC) { return; }
	BoundASC = ASC;

	AppliedHandle = ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
		this, &UGECallbacksComponent::OnAnyGEApplied);
	AddedHandle = ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		this, &UGECallbacksComponent::OnGEAdded);
	RemovedHandle = ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(
		this, &UGECallbacksComponent::OnGERemoved);

	// 属性变化委托：UI 绑定首选
	AttrHandle = ASC->GetGameplayAttributeValueChangeDelegate(
		UHealthSet::GetHealthAttribute()).AddUObject(
			this, &UGECallbacksComponent::OnHealthChanged);

	GAS_LOG("【GE回调】已绑定 4 类委托");
}

void UGECallbacksComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (BoundASC.IsValid())
	{
		// 解绑（生产代码必须做，否则 Actor 销毁后回调失败崩）
		if (AppliedHandle.IsValid()) BoundASC->OnGameplayEffectAppliedDelegateToSelf.Remove(AppliedHandle);
		if (AddedHandle.IsValid())   BoundASC->OnActiveGameplayEffectAddedDelegateToSelf.Remove(AddedHandle);
		if (RemovedHandle.IsValid()) BoundASC->OnAnyGameplayEffectRemovedDelegate().Remove(RemovedHandle);
		if (AttrHandle.IsValid())
		{
			BoundASC->GetGameplayAttributeValueChangeDelegate(
				UHealthSet::GetHealthAttribute()).Remove(AttrHandle);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UGECallbacksComponent::OnAnyGEApplied(UAbilitySystemComponent* Source,
	const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	GAS_LOG("【GE回调】OnApplied ：%s（仅 Applied 不一定进 Active）",
		*Spec.Def->GetName());
}

void UGECallbacksComponent::OnGEAdded(UAbilitySystemComponent* Source,
	const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	GAS_LOG("【GE回调】OnAdded ：%s 进入 Active", *Spec.Def->GetName());
}

void UGECallbacksComponent::OnGERemoved(const FGameplayEffectRemovalInfo& Info)
{
	GAS_LOG("【GE回调】OnRemoved ：提前移除=%d StackCount=%d",
		Info.bPrematureRemoval ? 1 : 0, Info.StackCount);
}

void UGECallbacksComponent::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	GAS_LOG("【UI 绑定】Health: %.1f -> %.1f", Data.OldValue, Data.NewValue);
	// 实战 ：在此触发 HealthBar Widget 更新百分比
}
