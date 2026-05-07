#include "P38_39_LifecycleCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "../00_Common/FundP2LogMacros.h"

AP38_LifecycleCharacter::AP38_LifecycleCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AP38_LifecycleCharacter::BeginPlay()
{
	Super::BeginPlay();
	P2_LOG("P38: Character::BeginPlay");

	// P39 ：把 DefaultMappingContext 添加到 EnhancedInputSubsystem
	// 必须在 PC 拥有本 Pawn 之后做（PossessedBy 时机也行）
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, /*Priority=*/0);
			}
		}
	}
}

void AP38_LifecycleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	P2_LOG("P38: PossessedBy %s", NewController ? *NewController->GetName() : TEXT("null"));
}

void AP38_LifecycleCharacter::UnPossessed()
{
	P2_LOG("P38: UnPossessed");
	Super::UnPossessed();
}

void AP38_LifecycleCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	P2_LOG("P38: OnRep_Controller （Client 收到 Possess 同步）");
}

void AP38_LifecycleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 必须 cast 到 EnhancedInputComponent 才有 BindAction
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)
		{
			EIC->BindAction(IA_Move, ETriggerEvent::Triggered,
				this, &AP38_LifecycleCharacter::OnMove);
		}
		if (IA_Jump)
		{
			EIC->BindAction(IA_Jump, ETriggerEvent::Started,
				this, &ACharacter::Jump);
			EIC->BindAction(IA_Jump, ETriggerEvent::Completed,
				this, &ACharacter::StopJumping);
		}
		if (IA_Attack)
		{
			// Started ：按下瞬间触发
			EIC->BindAction(IA_Attack, ETriggerEvent::Started,
				this, &AP38_LifecycleCharacter::OnAttackStart);
			// Completed ：按键结束（释放 / 取消）
			EIC->BindAction(IA_Attack, ETriggerEvent::Completed,
				this, &AP38_LifecycleCharacter::OnAttackComplete);
		}
	}
}

void AP38_LifecycleCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (Controller && !Axis.IsNearlyZero())
	{
		const FRotator Rot(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Forward = FRotationMatrix(Rot).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(Rot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Forward, Axis.Y);
		AddMovementInput(Right,   Axis.X);
	}
}

void AP38_LifecycleCharacter::OnAttackStart(const FInputActionValue& Value)
{
	P2_LOG("P39: Attack Started");
}

void AP38_LifecycleCharacter::OnAttackComplete(const FInputActionValue& Value)
{
	P2_LOG("P39: Attack Completed");
}
