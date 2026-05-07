#include "P73_NavMeshHelper.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "../00_Common/FundP3LogMacros.h"

bool UP73_NavMeshHelper::MoveAIToActor(AAIController* AICon, AActor* Target, float AcceptanceRadius)
{
	if (!AICon || !Target) { return false; }
	const EPathFollowingRequestResult::Type Res =
		AICon->MoveToActor(Target, AcceptanceRadius);
	P3_LOG("P73: MoveToActor %s -> %d",
		*Target->GetName(), static_cast<int32>(Res));
	return Res != EPathFollowingRequestResult::Failed;
}

bool UP73_NavMeshHelper::MoveAIToLocation(AAIController* AICon, FVector Location, float AcceptanceRadius)
{
	if (!AICon) { return false; }
	const EPathFollowingRequestResult::Type Res =
		AICon->MoveToLocation(Location, AcceptanceRadius);
	return Res != EPathFollowingRequestResult::Failed;
}

bool UP73_NavMeshHelper::GetRandomNavLocation(const UObject* WorldContext, FVector Origin, float Radius, FVector& OutLocation)
{
	if (!WorldContext) { return false; }
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(WorldContext->GetWorld());
	if (!NavSys) { return false; }

	FNavLocation Result;
	const bool bOK = NavSys->GetRandomReachablePointInRadius(Origin, Radius, Result);
	if (bOK) { OutLocation = Result.Location; }
	return bOK;
}
