// P70 - Behavior Tree Task ：在 Blackboard 写入最近敌人
//
// 用法 ：
//   1) BT 资产里加节点 ：BTTask_FindTarget
//   2) Details 设 TargetKey = Blackboard 中的 Object 类型 Key
//   3) ExecuteTask 返回 Succeeded / Failed
//
// AIController 必须 ：
//   * AIPerception 检测到敌人 ；或 AIController 自定义 KnownEnemies 列表

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "P70_BTTask_FindTarget.generated.h"

UCLASS()
class UETEST_API UP70_BTTask_FindTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UP70_BTTask_FindTarget();

	// Blackboard Key Selector ：编辑器中下拉选择目标 Key（Object 类型）
	UPROPERTY(EditAnywhere, Category = "P70")
	FBlackboardKeySelector TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
