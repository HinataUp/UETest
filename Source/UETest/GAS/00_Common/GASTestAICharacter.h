// 公共 AI Character ：ASC 在 Character 上（不在 PlayerState）
// I43 - 该 ASC 用 Minimal 复制模式（只发必要 Tag/属性，省带宽）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GASTestAICharacter.generated.h"

class UAbilitySystemComponent;
class UHealthSet;
class UCombatSet;

UCLASS()
class UETEST_API AGASTestAICharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASTestAICharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UHealthSet* GetHealthSet() const { return HealthSet; }
	UCombatSet* GetCombatSet() const { return CombatSet; }

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UCombatSet> CombatSet;
};
