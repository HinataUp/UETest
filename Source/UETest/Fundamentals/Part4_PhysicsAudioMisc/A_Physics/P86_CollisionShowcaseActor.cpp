#include "P86_CollisionShowcaseActor.h"
#include "Components/SphereComponent.h"
#include "../00_Common/FundP4LogMacros.h"

AP86_CollisionShowcaseActor::AP86_CollisionShowcaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(50.f);
	RootComponent = SphereComp;

	// 方式1 ：用预设 Profile（推荐 ：编辑器可视化配置）
	// SphereComp->SetCollisionProfileName(FName("BlockAllDynamic"));

	// 方式2 ：手动配置（演示用）
	// 自身类型 ：可在 Project Settings 添加 ECC_GameTraceChannel1 当 Projectile
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);

	// 默认全部 Ignore ：再开启想要的几条
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 对 Pawn ：Block（撞 Player 时触发 OnHit）
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	// 对 WorldStatic ：Block
	SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// 对 WorldDynamic ：Overlap（穿透但触发 OnOverlap）
	SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// 必须 Generate 才会触发 Overlap 事件
	SphereComp->SetGenerateOverlapEvents(true);
}

void AP86_CollisionShowcaseActor::BeginPlay()
{
	Super::BeginPlay();

	// 绑定回调 ：BeginPlay 比构造期更稳（避免编辑器期触发）
	SphereComp->OnComponentHit.AddDynamic(this, &AP86_CollisionShowcaseActor::OnSphereHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AP86_CollisionShowcaseActor::OnSphereOverlap);
}

void AP86_CollisionShowcaseActor::OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	P4_LOG("P86: OnHit %s （Block 响应）",
		OtherActor ? *OtherActor->GetName() : TEXT("null"));
}

void AP86_CollisionShowcaseActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	P4_LOG("P86: OnOverlap %s （Overlap 响应）",
		OtherActor ? *OtherActor->GetName() : TEXT("null"));
}
