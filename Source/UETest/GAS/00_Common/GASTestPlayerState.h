// 公共测试 PlayerState：拥有 ASC + 两个 AttributeSet
//
// 设计：
//   * Player 的 ASC 放在 PlayerState（这是 Lyra / 官方推荐做法）
//   * ReplicationMode = Mixed ：Owner 收完整 GE 信息，Simulated 只收 Tag/属性
//   * AttributeSet 也作为 DefaultSubobject 创建在 PlayerState 上

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GASTestPlayerState.generated.h"

class UAbilitySystemComponent;
class UHealthSet;
class UCombatSet;

UCLASS()
class UETEST_API AGASTestPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASTestPlayerState();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 业务便捷取值
	UHealthSet* GetHealthSet() const { return HealthSet; }
	UCombatSet* GetCombatSet() const { return CombatSet; }

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UCombatSet> CombatSet;
};
