#include "P09_DamageableActor.h"

AP09_DamageableActor::AP09_DamageableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

float AP09_DamageableActor::TakeDamage_Implementation(float Amount)
{
	Health = FMath::Max(0.f, Health - Amount);
	return Health;
}
