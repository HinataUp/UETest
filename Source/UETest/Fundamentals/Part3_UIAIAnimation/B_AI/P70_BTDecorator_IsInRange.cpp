#include "P70_BTDecorator_IsInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UP70_BTDecorator_IsInRange::UP70_BTDecorator_IsInRange()
{
	NodeName = TEXT("Is In Range");
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UP70_BTDecorator_IsInRange, TargetKey),
		AActor::StaticClass());
}

bool UP70_BTDecorator_IsInRange::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) { return false; }
	APawn* Self = AICon->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Self || !BB) { return false; }

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) { return false; }

	const float D = FVector::Dist(Self->GetActorLocation(), Target->GetActorLocation());
	return D <= Range;
}
