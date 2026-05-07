// P38 - Character ：Pawn + CapsuleComponent + CharacterMovementComponent + SkeletalMesh
// P39 - Enhanced Input ：UInputAction + UInputMappingContext + ETriggerEvent
//
// 设计 ：合并到一个 Character ；BindAction 用 EnhancedInputComponent

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "P38_39_LifecycleCharacter.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class UETEST_API AP38_LifecycleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AP38_LifecycleCharacter();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 编辑器配置 ：BP 中拖入 IA / IMC 资产
	UPROPERTY(EditDefaultsOnly, Category = "P39|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "P39|Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "P39|Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "P39|Input")
	TObjectPtr<UInputAction> IA_Attack;

protected:
	void OnMove(const FInputActionValue& Value);
	void OnAttackStart(const FInputActionValue& Value);
	void OnAttackComplete(const FInputActionValue& Value);
};
