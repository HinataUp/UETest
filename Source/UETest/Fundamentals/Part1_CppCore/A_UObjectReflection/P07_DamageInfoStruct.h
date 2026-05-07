// P07 - USTRUCT ：值类型与 NetSerialize
//
// USTRUCT 是值类型 ：栈分配 / 拷贝语义 / 不参与 GC
// BlueprintType + UPROPERTY 字段 ：BP 可编辑 / 可作为 BP 变量
// 自定义 NetSerialize ：可压缩网络包

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P07_DamageInfoStruct.generated.h"

USTRUCT(BlueprintType)
struct UETEST_API FP07_DamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DamageType;

	// TWeakObjectPtr 可在 USTRUCT 中安全使用（不阻止 GC）
	UPROPERTY()
	TWeakObjectPtr<AActor> Instigator;

	bool operator==(const FP07_DamageInfo& Other) const
	{
		return FMath::IsNearlyEqual(Amount, Other.Amount)
			&& DamageType == Other.DamageType
			&& Instigator == Other.Instigator;
	}

	// 自定义 NetSerialize ：可省略 Instigator 等不重要字段 / 量化数值
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

// 让 UE 反射系统知道我们 override 了 NetSerialize
template<>
struct TStructOpsTypeTraits<FP07_DamageInfo> : public TStructOpsTypeTraitsBase2<FP07_DamageInfo>
{
	enum
	{
		WithNetSerializer = true,
		WithIdenticalViaEquality = true,
	};
};
