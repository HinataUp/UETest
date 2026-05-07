// D24 - Execution Calculation：工业级伤害公式骨架
//
// 与 MMC 的区别：
//   * MMC：产出一个 float，给单个 Modifier 用
//   * ExecCalc：可读多属性、写多属性，是完整的"计算 Pass"
//
// 仅可用于 Instant / Periodic GE（不能用于 Duration / Infinite）

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "D24_ExecCalc_Damage.generated.h"

UCLASS()
class UETEST_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();
	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
