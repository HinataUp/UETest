#include "P78_MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UP78_MyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UP78_MyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn) { return; }

	// Speed ：忽略垂直速度的水平速率
	const FVector Vel = Pawn->GetVelocity();
	Speed = Vel.Size2D();

	// Direction ：用 KismetMathLibrary 的角度计算 ；返回 -180 ~ 180
	Direction = UKismetMathLibrary::CalculateDirection(Vel, Pawn->GetActorRotation());

	// 状态 ：CharacterMovement 直接读
	if (ACharacter* Char = Cast<ACharacter>(Pawn))
	{
		bIsInAir = Char->GetCharacterMovement()->IsFalling();
		bIsCrouching = Char->bIsCrouched;
	}
}
