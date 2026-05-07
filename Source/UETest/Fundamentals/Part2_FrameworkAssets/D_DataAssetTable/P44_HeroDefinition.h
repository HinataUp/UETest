// P44 - UPrimaryDataAsset ：被 AssetManager 管理的只读配置数据
//
// UDataAsset       ：基础只读数据载体 ；不参与 AssetManager 扫描
// UPrimaryDataAsset ：override GetPrimaryAssetId 后 ，AssetManager 可扫描 / 异步加载 / Bundle

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/SkeletalMesh.h"
#include "P44_HeroDefinition.generated.h"

UCLASS(BlueprintType)
class UETEST_API UP44_HeroDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BaseHealth = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BaseAttack = 20;

	// P51 ：用 AssetBundles 把 Mesh / Icon 分组 ；按需加载
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (AssetBundles = "Gameplay"))
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (AssetBundles = "UI"))
	TSoftObjectPtr<class UTexture2D> Icon;

	// 关键 ：让 AssetManager 能扫描到本类型
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Hero"), GetFName());
	}
};
