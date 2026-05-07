// A02 - 第二个 AttributeSet：战斗属性
// D26 - 暴击属性也在此 Set
// I44 - Mana / Experience 演示 OwnerOnly 复制条件
// H42 - LifeStealRate / ThornsRate 演示跨目标效果
//
// 拆分原则：HealthSet 关注生命，CombatSet 关注输出/防御。每 Set 5-15 个属性较合适。

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "A01_HealthSet.h"  // 复用 ATTRIBUTE_ACCESSORS 宏
#include "A02_CombatSet.generated.h"

UCLASS()
class UETEST_API UCombatSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCombatSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 公开属性 ：所有玩家都能看到
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UCombatSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UCombatSet, Defense)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritRate)
	FGameplayAttributeData CritRate;
	ATTRIBUTE_ACCESSORS(UCombatSet, CritRate)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage)
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UCombatSet, CritDamage)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeStealRate)
	FGameplayAttributeData LifeStealRate;
	ATTRIBUTE_ACCESSORS(UCombatSet, LifeStealRate)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ThornsRate)
	FGameplayAttributeData ThornsRate;
	ATTRIBUTE_ACCESSORS(UCombatSet, ThornsRate)

	// I44 私有属性 ：只 Owner 端可见
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCombatSet, Mana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCombatSet, MaxMana)

protected:
	UFUNCTION() void OnRep_AttackPower(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_Defense(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_CritRate(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_CritDamage(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_LifeStealRate(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_ThornsRate(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& Old);
	UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& Old);
};
