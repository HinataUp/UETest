// P70 - Behavior Tree Decorator ：检查目标是否在范围内
// CalculateRawConditionValue 返回 true 时分支执行 ，false 时跳过

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "P70_BTDecorator_IsInRange.generated.h"

UCLASS()
class UETEST_API UP70_BTDecorator_IsInRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UP70_BTDecorator_IsInRange();

	UPROPERTY(EditAnywhere, Category = "P70")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "P70", meta = (ClampMin = "0"))
	float Range = 300.f;

protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
