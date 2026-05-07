// 测试用极简 Actor ：ASC + HealthSet + CombatSet 自包含
//
// 用途：
//   * 自动化 Spec 测试中 NewObject 即得，无需完整 PlayerState 生命周期
//   * 也可拖到关卡作为靶子目标 ( target dummy ) 来手动 PIE 测 GE 效果
//
// 与 GASTestPlayerState 的区别：
//   * PlayerState 走 Mixed 复制 + Pawn 关联，更接近真实玩家
//   * 本 Actor 用作 minimal 容器，方便单测和打靶子

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GASTestActor.generated.h"

class UAbilitySystemComponent;
class UHealthSet;
class UCombatSet;

UCLASS()
class UETEST_API AGASTestActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASTestActor();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UHealthSet* GetHealthSet() const { return HealthSet; }
	UCombatSet* GetCombatSet() const { return CombatSet; }

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UCombatSet> CombatSet;
};
