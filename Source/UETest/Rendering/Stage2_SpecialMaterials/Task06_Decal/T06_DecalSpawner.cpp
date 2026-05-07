#include "T06_DecalSpawner.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../../00_Common/RenderingLogMacros.h"

UT06_DecalSpawner::UT06_DecalSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UDecalComponent* UT06_DecalSpawner::SpawnBulletHole(const FVector& Location, const FVector& Normal)
{
	if (!BulletDecalMaterial || !GetWorld())
	{
		RT_LOG("Decal: 缺少材质或 World");
		return nullptr;
	}

	// 关键 ：Decal Component 的 -X 轴是投影方向 ；Normal 朝外 -> Rotation 让 -X 朝 Normal 反向
	const FRotator Rotation = (-Normal).Rotation();

	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		BulletDecalMaterial,
		DecalSize,
		Location,
		Rotation,
		LifeSpan);

	if (Decal)
	{
		// 设置淡入淡出 ：UE5 内置 SetFadeIn / SetFadeOut
		Decal->SetFadeOut(FMath::Max(0.f, LifeSpan - FadeDuration), FadeDuration, /*bDestroyAfterFade=*/true);
		RT_LOG("Decal 已生成 @ (%.1f,%.1f,%.1f)", Location.X, Location.Y, Location.Z);
	}
	return Decal;
}
