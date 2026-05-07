// A01 + A05 + D22 - HealthSet 实现

#include "A01_HealthSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "../00_Common/GASLogMacros.h"
#include "../00_Common/MyGameplayTags.h"

UHealthSet::UHealthSet()
{
	// 给个默认值，方便单测无 GE 时也有意义
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitIncomingDamage(0.f);
}

void UHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 标准复制套路：DOREPLIFETIME_CONDITION_NOTIFY + REPNOTIFY_Always
	// REPNOTIFY_Always 让"快速变化但落回原值"的场景仍触发 OnRep
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, Health,    COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
	// IncomingDamage 是 Meta，不复制
}

void UHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	// 必须调用此宏：内部会通知 ASC 的聚合器把这次复制纳入 CurrentValue 计算
	// 漏掉这一行 → Client 端的 Modifier 聚合不会更新，UI 拿到的值会"卡住"
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, Health, OldHealth);
}

void UHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, MaxHealth, OldMaxHealth);
}

// ───────────────── A05 Hook 演示 ─────────────────

void UHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	bDebugPreChangeFired = true;

	// 标准 Clamp 模式：所有路径（GE / 直接 Set / Modifier 聚合）都走这里
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealth())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}

	GAS_VLOG("PreAttributeChange %s -> %.1f",
		*Attribute.AttributeName, NewValue);
}

void UHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	bDebugPostExecuteFired = true;

	// D22 Meta Attribute 标准模式：把 IncomingDamage 转为 Health 扣减
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncoming = GetIncomingDamage();
		SetIncomingDamage(0.f); // 用完即重置，下次还能写入

		if (LocalIncoming > 0.f)
		{
			const float NewHealth = FMath::Max(0.f, GetHealth() - LocalIncoming);
			SetHealth(NewHealth);
			GAS_LOG("【伤害管线】受到伤害 %.1f, 剩余 Health %.1f", LocalIncoming, NewHealth);
		}
	}

	// A05 死亡判定 ：放在 Post 不放 Pre —— Pre 还在 Clamp 中，时序错乱
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= 0.f)
		{
			// 实际项目此处 Apply 一个 GE_Dead，授予 State.Dead Tag、播死亡 Cue 等
			GAS_LOG("【死亡判定】Health <= 0, 触发死亡逻辑");
		}
	}
}
