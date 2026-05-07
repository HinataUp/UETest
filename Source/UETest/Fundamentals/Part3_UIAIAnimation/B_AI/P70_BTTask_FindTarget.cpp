#include "P70_BTTask_FindTarget.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "../00_Common/FundP3LogMacros.h"

UP70_BTTask_FindTarget::UP70_BTTask_FindTarget()
{
	NodeName = TEXT("Find Target");
	// 限定 Key Selector 只能选 Object 类型
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UP70_BTTask_FindTarget, TargetKey),
		AActor::StaticClass());
}

EBTNodeResult::Type UP70_BTTask_FindTarget::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) { return EBTNodeResult::Failed; }

	APawn* Self = AICon->GetPawn();
	if (!Self) { return EBTNodeResult::Failed; }

	// 找最近的 ACharacter（实战项目应该按 Team / Tag 过滤）
	AActor* Closest = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();
	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		ACharacter* C = *It;
		if (!C || C == Self) { continue; }
		const float D = FVector::DistSquared(C->GetActorLocation(), Self->GetActorLocation());
		if (D < BestDistSq)
		{
			BestDistSq = D;
			Closest = C;
		}
	}

	if (!Closest)
	{
		P3_LOG("P70: FindTarget Failed");
		return EBTNodeResult::Failed;
	}

	// 写入 Blackboard
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsObject(TargetKey.SelectedKeyName, Closest);
	}
	P3_LOG("P70: FindTarget %s", *Closest->GetName());
	return EBTNodeResult::Succeeded;
}
