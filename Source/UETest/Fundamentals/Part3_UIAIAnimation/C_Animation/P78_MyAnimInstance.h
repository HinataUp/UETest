// P78 - AnimInstance 基础 ：C++ 驱动 AnimBP
//
// NativeUpdateAnimation 每帧 ：从 Owner Pawn 拉数据 -> 写入 BlueprintReadOnly 字段
// AnimBP 中读取这些字段驱动 BlendSpace / State Machine
//
// 编辑器步骤 ：
//   1) New Anim Blueprint → Parent: P78_MyAnimInstance
//   2) AnimGraph 中拖入 BlendSpace ；Speed / Direction 输入接 P78 字段

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P78_MyAnimInstance.generated.h"

UCLASS()
class UETEST_API UP78_MyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// AnimBP 中读取的字段 ：BlueprintReadOnly + 默认 ；驱动 State Machine / BlendSpace
	UPROPERTY(BlueprintReadOnly, Category = "P78|Movement")
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "P78|Movement")
	float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "P78|Movement")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "P78|Movement")
	bool bIsCrouching = false;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
