#include "P87_TraceHelper.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "../00_Common/FundP4LogMacros.h"

bool UP87_TraceHelper::LineTraceVisibility(
	const UObject* WorldContext, FVector Start, FVector End,
	FHitResult& OutHit, AActor* IgnoreActor, bool bDrawDebug)
{
	if (!WorldContext) { return false; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return false; }

	FCollisionQueryParams Params(SCENE_QUERY_STAT(P87_Line), /*bTraceComplex=*/false);
	if (IgnoreActor) { Params.AddIgnoredActor(IgnoreActor); }

	const bool bHit = World->LineTraceSingleByChannel(
		OutHit, Start, End, ECC_Visibility, Params);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugLine(World, Start, bHit ? OutHit.ImpactPoint : End,
			bHit ? FColor::Red : FColor::Green, false, 1.f, 0, 1.5f);
	}
#endif

	return bHit;
}

bool UP87_TraceHelper::SphereSweep(
	const UObject* WorldContext, FVector Start, FVector End,
	float Radius, FHitResult& OutHit, AActor* IgnoreActor, bool bDrawDebug)
{
	if (!WorldContext) { return false; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return false; }

	FCollisionQueryParams Params(SCENE_QUERY_STAT(P87_Sweep), /*bTraceComplex=*/false);
	if (IgnoreActor) { Params.AddIgnoredActor(IgnoreActor); }

	const FCollisionShape Shape = FCollisionShape::MakeSphere(FMath::Max(0.f, Radius));
	const bool bHit = World->SweepSingleByChannel(
		OutHit, Start, End, FQuat::Identity, ECC_Pawn, Shape, Params);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugSphere(World, Start, Radius, 16, FColor::Yellow, false, 1.f);
		DrawDebugSphere(World, End, Radius, 16,
			bHit ? FColor::Red : FColor::Green, false, 1.f);
		DrawDebugLine(World, Start, End, FColor::Cyan, false, 1.f, 0, 1.f);
	}
#endif

	return bHit;
}

int32 UP87_TraceHelper::SphereOverlapPawns(
	const UObject* WorldContext, FVector Origin, float Radius,
	TArray<AActor*>& OutActors, bool bDrawDebug)
{
	OutActors.Reset();
	if (!WorldContext) { return 0; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return 0; }

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(P87_Overlap), /*bTraceComplex=*/false);
	const FCollisionShape Shape = FCollisionShape::MakeSphere(FMath::Max(0.f, Radius));
	World->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, Shape, Params);

	for (const FOverlapResult& O : Overlaps)
	{
		if (AActor* A = O.GetActor())
		{
			OutActors.AddUnique(A);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugSphere(World, Origin, Radius, 24,
			OutActors.Num() > 0 ? FColor::Red : FColor::Green, false, 1.f);
	}
#endif

	P4_LOG("P87: Overlap 命中 %d 个 Pawn", OutActors.Num());
	return OutActors.Num();
}
