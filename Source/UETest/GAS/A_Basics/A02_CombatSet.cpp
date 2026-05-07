// A02 - CombatSet 实现

#include "A02_CombatSet.h"
#include "Net/UnrealNetwork.h"

UCombatSet::UCombatSet()
{
	InitAttackPower(20.f);
	InitDefense(0.f);
	InitCritRate(0.f);
	InitCritDamage(1.5f);
	InitLifeStealRate(0.f);
	InitThornsRate(0.f);
	InitMana(100.f);
	InitMaxMana(100.f);
}

void UCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 公共属性 ：全部 Client 都能看到
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, AttackPower,    COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, Defense,        COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, CritRate,       COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, CritDamage,     COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, LifeStealRate,  COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, ThornsRate,     COND_None, REPNOTIFY_Always);

	// I44 私有属性 ：只 Owner 端可见
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, Mana,           COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, MaxMana,        COND_OwnerOnly, REPNOTIFY_Always);
}

// 所有 OnRep 标准实现 ：调 GAMEPLAYATTRIBUTE_REPNOTIFY 触发 ASC 聚合刷新
void UCombatSet::OnRep_AttackPower(const FGameplayAttributeData& Old)   { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, AttackPower, Old); }
void UCombatSet::OnRep_Defense(const FGameplayAttributeData& Old)       { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, Defense, Old); }
void UCombatSet::OnRep_CritRate(const FGameplayAttributeData& Old)      { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, CritRate, Old); }
void UCombatSet::OnRep_CritDamage(const FGameplayAttributeData& Old)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, CritDamage, Old); }
void UCombatSet::OnRep_LifeStealRate(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, LifeStealRate, Old); }
void UCombatSet::OnRep_ThornsRate(const FGameplayAttributeData& Old)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, ThornsRate, Old); }
void UCombatSet::OnRep_Mana(const FGameplayAttributeData& Old)          { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, Mana, Old); }
void UCombatSet::OnRep_MaxMana(const FGameplayAttributeData& Old)       { GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet, MaxMana, Old); }
