// P83 - Control Rig Helper（运行时驱动 Rig 参数）
//
// 注意 ：完整 Control Rig 需要 ControlRig 模块依赖 ；本 helper 通过 AnimInstance
//   持有 RigInstance 来设置 ；适合实战参考。
// 编辑器 ：创建 ControlRig 资产 → 在 AnimBP 中加 Control Rig 节点引用此 Rig

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P83_ControlRigHelper.generated.h"

class USkeletalMeshComponent;

// 思路 ：本组件挂在 Character 上 ，提供 BP 友好接口设置 AnimInstance 中暴露的 Float / Vector 字段
// （通过 BP 调 ：拿到 AnimInstance 后 SetVariableXxxByName）

UCLASS(ClassGroup = (P3), meta = (BlueprintSpawnableComponent))
class UETEST_API UP83_ControlRigHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	UP83_ControlRigHelper();

	// 给 Owner 角色的 AnimInstance 设置一个 float 变量（BP 中读取后传给 Control Rig 节点）
	UFUNCTION(BlueprintCallable, Category = "P83|ControlRig")
	void SetAnimFloat(FName VariableName, float Value);

	UFUNCTION(BlueprintCallable, Category = "P83|ControlRig")
	void SetAnimVector(FName VariableName, FVector Value);

private:
	USkeletalMeshComponent* GetOwnerMesh() const;
};
