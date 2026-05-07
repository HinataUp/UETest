// A01 - 第一个 AttributeSet: Health + MaxHealth
// A05 - PreAttributeChange 与 PostGameplayEffectExecute Hook 也在此 Set 演示
// D22 - Meta Attribute (IncomingDamage) 也在此 Set 演示，避免再开一个 Set
//
// 关键点：
//   * FGameplayAttributeData 双值结构 ：BaseValue（永久存储）+ CurrentValue（含 Modifier 聚合）
//   * ATTRIBUTE_ACCESSORS 宏一次生成 Getter / Setter / Initter / GetXXXAttribute() 四个函数
//   * Replicated + OnRep 内必须调 GAMEPLAYATTRIBUTE_REPNOTIFY ，否则 Client 端聚合器不刷新

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "A01_HealthSet.generated.h"

// 标准宏：一次生成 GetXxxAttribute / GetXxx / SetXxx / InitXxx 四个静态函数
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UETEST_API UHealthSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHealthSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// A05 - 两个 Hook
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// 标准复制属性：Health + MaxHealth
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthSet, MaxHealth)

	// D22 Meta Attribute：不复制、不持久；GE 写入 -> PostExecute 计算 -> 重置为 0
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UHealthSet, IncomingDamage)

	// 测试桩：让单测 / 业务代码能观察 Hook 是否触发过
	mutable bool bDebugPostExecuteFired = false;
	mutable bool bDebugPreChangeFired = false;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};
