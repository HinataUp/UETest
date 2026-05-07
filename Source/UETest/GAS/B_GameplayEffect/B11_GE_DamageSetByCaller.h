// B11 - SetByCaller 动态注入 Magnitude
// Magnitude Type 设为 SetByCaller，代码中通过 Tag 注入数值
// 用法：Spec.Data->SetSetByCallerMagnitude(TAG_SetByCaller_Damage, -40.f)

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B11_GE_DamageSetByCaller.generated.h"

UCLASS()
class UETEST_API UGE_DamageSetByCaller : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_DamageSetByCaller();
};
