#include "H42_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../00_Common/GASLogMacros.h"

AGASProjectile::AGASProjectile()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(20.f);
	Sphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Sphere;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1500.f;
	Movement->MaxSpeed = 1500.f;
	Movement->ProjectileGravityScale = 0.f;
	Movement->SetIsReplicated(true);

	// 让位置 / 朝向自动复制
	SetReplicateMovement(true);
}

void AGASProjectile::MarkAsVisualOnly()
{
	// 标志位 ：本地视觉球
	bIsVisualOnly = true;

	// 关闭复制 ：纯本地
	SetReplicates(false);
	SetReplicateMovement(false);

	// 关闭命中 ：由 Server 权威球判定
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AGASProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsVisualOnly && HasAuthority())
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGASProjectile::OnSphereOverlap);
	}

	GAS_LOG("【投射物】%s BeginPlay isVisualOnly=%d",
		*GetName(), bIsVisualOnly ? 1 : 0);
}

void AGASProjectile::OnSphereOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (!HasAuthority() || !OtherActor || OtherActor == this) { return; }

	GAS_LOG("【投射物】Server 命中 %s ；可在此 ApplyGE + ExecuteGameplayCue", *OtherActor->GetName());
	Destroy();
}
