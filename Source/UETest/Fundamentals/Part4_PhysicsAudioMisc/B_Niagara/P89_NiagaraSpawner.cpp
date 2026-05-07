#include "P89_NiagaraSpawner.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../00_Common/FundP4LogMacros.h"

UP89_NiagaraSpawner::UP89_NiagaraSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UNiagaraComponent* UP89_NiagaraSpawner::SpawnExplosion(FVector Location, FVector Normal,
	float Scale, FLinearColor Color)
{
	if (!ExplosionSystem || !GetWorld()) { return nullptr; }

	UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), ExplosionSystem,
		Location, Normal.Rotation(),
		FVector(Scale), /*bAutoDestroy=*/true);

	if (NC)
	{
		// 设置 User Parameter ：Niagara System 中以 "User.Color" / "User.Scale" 暴露
		NC->SetVariableLinearColor(FName("Color"), Color);
		NC->SetVariableFloat(FName("Scale"), Scale);

		// 完成回调 ：bAutoDestroy=true 时，结束自动销毁
		NC->OnSystemFinished.AddDynamic(this, &UP89_NiagaraSpawner::HandleExplosionFinished);
	}
	return NC;
}

UNiagaraComponent* UP89_NiagaraSpawner::AttachBurning(USceneComponent* AttachTo)
{
	if (!BurningSystem || !AttachTo) { return nullptr; }

	UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAttached(
		BurningSystem, AttachTo,
		NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget, /*bAutoDestroy=*/false);

	P4_LOG("P89: 附加燃烧 VFX 到 %s", *AttachTo->GetName());
	return NC;
}

void UP89_NiagaraSpawner::HandleExplosionFinished(UNiagaraComponent* PSystem)
{
	P4_LOG("P89: 爆炸 VFX 播放完成");
	OnExplosionFinished.Broadcast();
}
