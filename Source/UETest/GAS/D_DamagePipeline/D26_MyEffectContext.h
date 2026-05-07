// D26 - 自定义 GameplayEffectContext：携带暴击信息到 Cue/UI 层
//
// 默认 Context 字段有限（Instigator、HitResult），传暴击/元素类型需要扩展
// 关键：必须 override NetSerialize / Duplicate 才能跨网络

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "D26_MyEffectContext.generated.h"

USTRUCT(BlueprintType)
struct UETEST_API FMyGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	bool IsCriticalHit() const { return bIsCrit; }
	void SetIsCriticalHit(bool bNew) { bIsCrit = bNew; }

	// FGameplayEffectContext API
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	// 自定义字段：必须实现 NetSerialize 才能跨端
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	// 复制时复制内部字段
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FMyGameplayEffectContext* NewCtx = new FMyGameplayEffectContext();
		*NewCtx = *this;
		NewCtx->AddActors(GetActors());
		if (GetHitResult())
		{
			NewCtx->AddHitResult(*GetHitResult(), true);
		}
		return NewCtx;
	}

protected:
	UPROPERTY()
	bool bIsCrit = false;
};

// 让 TStructOpsTypeTraits 知道我们 override 了 NetSerialize
template<>
struct TStructOpsTypeTraits<FMyGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FMyGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
