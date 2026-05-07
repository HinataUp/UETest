// P17-P20 - 委托与事件
//
// P17 单播委托 ：DECLARE_DELEGATE + Bind* / Execute / ExecuteIfBound
// P18 多播委托 ：MULTICAST（C++ only）+ DYNAMIC_MULTICAST（BP 可见）
// P19 Delegate Handle ：精确 Remove / RemoveAll
// P20 返回值委托 + Payload ：单播可有返回值 ；CreateUObject 带额外参

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P17_20_DelegateShowcase.generated.h"

// ───────────── 单播 ：可有返回值 ─────────────
DECLARE_DELEGATE_OneParam(FP17_OnDamage, float /*Amount*/);
DECLARE_DELEGATE_RetVal_OneParam(bool, FP20_CanActivate, int32 /*AbilityID*/);

// ───────────── 多播 ：C++ only ─────────────
DECLARE_MULTICAST_DELEGATE_OneParam(FP18_OnHealthChanged, float /*NewHealth*/);

// ───────────── 动态多播 ：BP 可见（可 BlueprintAssignable） ─────────────
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP18_OnDeathSignature, AActor*, Killer);

UCLASS()
class UETEST_API AP17_20_DelegateShowcase : public AActor
{
	GENERATED_BODY()

public:
	AP17_20_DelegateShowcase();

	// 单播
	FP17_OnDamage OnDamage;

	// 多播 C++（蓝图不可见）
	FP18_OnHealthChanged OnHealthChanged;

	// 动态多播 ：UPROPERTY(BlueprintAssignable) BP 可见 / 可绑定
	UPROPERTY(BlueprintAssignable, Category = "P17-20")
	FP18_OnDeathSignature OnDeath;

	// 返回值单播
	FP20_CanActivate CanActivateCheck;

	// 业务方法 ：用来在测试中验证不同绑定路径
	UFUNCTION(BlueprintCallable, Category = "P17-20")
	void TriggerDamage(float Amount) { OnDamage.ExecuteIfBound(Amount); }

	UFUNCTION(BlueprintCallable, Category = "P17-20")
	void BroadcastHealth(float NewHealth) { OnHealthChanged.Broadcast(NewHealth); }

	UFUNCTION(BlueprintCallable, Category = "P17-20")
	void BroadcastDeath(AActor* Killer) { OnDeath.Broadcast(Killer); }
};
