#include "H41_ComboGA.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UGA_ComboAttack::UGA_ComboAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	AbilityTags.AddTag(MyGameplayTags::TAG_Ability_Skill_Combo);
}

void UGA_ComboAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData*)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;

	++ComboIndex;
	GAS_LOG("【连招】Combo%d 播放（动画 Notify 会发 Event.Combo.WindowOpen / Close）", ComboIndex);

	// 监听窗口开关 ：动画 Notify 通过 SendGameplayEventToActor 发出
	auto* OpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, MyGameplayTags::TAG_Event_Combo_WindowOpen);
	OpenTask->EventReceived.AddDynamic(this, &UGA_ComboAttack::OnComboWindowOpen);
	OpenTask->ReadyForActivation();

	auto* CloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, MyGameplayTags::TAG_Event_Combo_WindowClose);
	CloseTask->EventReceived.AddDynamic(this, &UGA_ComboAttack::OnComboWindowClose);
	CloseTask->ReadyForActivation();
}

void UGA_ComboAttack::OnComboWindowOpen(FGameplayEventData Payload)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		// 挂 Tag ：玩家下次按攻击时检查此 Tag 决定是否进入下一段
		ASC->AddLooseGameplayTag(MyGameplayTags::TAG_State_Combo_Ready);
	}
	GAS_LOG("【连招】窗口 Open ，Combo%d 可衔接", ComboIndex);
}

void UGA_ComboAttack::OnComboWindowClose(FGameplayEventData Payload)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(MyGameplayTags::TAG_State_Combo_Ready);
	}
	GAS_LOG("【连招】窗口 Close ，本段结束");
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}
