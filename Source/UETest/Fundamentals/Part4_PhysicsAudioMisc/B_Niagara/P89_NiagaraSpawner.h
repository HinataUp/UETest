// P89 - Niagara 基础 ：C++ 生成 + 参数控制
//
// 两种用法 ：
//   * SpawnSystemAtLocation     ：一次性 ；如爆炸 ；OnSystemFinished 回调
//   * SpawnSystemAttached       ：附加到 Component ；持续型 ；如燃烧 / 拖尾
//
// SetVariableXxx ：把 Niagara System 的 User Parameter 暴露给 C++ 控制

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P89_NiagaraSpawner.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FP89_OnVFXFinished);

UCLASS(ClassGroup = (P4), meta = (BlueprintSpawnableComponent))
class UETEST_API UP89_NiagaraSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	UP89_NiagaraSpawner();

	UPROPERTY(EditAnywhere, Category = "P89|VFX")
	TObjectPtr<UNiagaraSystem> ExplosionSystem;

	UPROPERTY(EditAnywhere, Category = "P89|VFX")
	TObjectPtr<UNiagaraSystem> BurningSystem;

	UPROPERTY(BlueprintAssignable)
	FP89_OnVFXFinished OnExplosionFinished;

	// 一次性 VFX ：在指定位置生成 ；OnExplosionFinished 在系统播放完成时触发
	UFUNCTION(BlueprintCallable, Category = "P89|VFX")
	UNiagaraComponent* SpawnExplosion(FVector Location, FVector Normal,
		float Scale = 1.f, FLinearColor Color = FLinearColor::White);

	// 持续 VFX ：附加到 SceneComponent ；返回 NC 句柄 ，外部可 Deactivate
	UFUNCTION(BlueprintCallable, Category = "P89|VFX")
	UNiagaraComponent* AttachBurning(USceneComponent* AttachTo);

protected:
	UFUNCTION()
	void HandleExplosionFinished(UNiagaraComponent* PSystem);
};
