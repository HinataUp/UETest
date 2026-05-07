#include "P97_BatchActorHelper.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "../00_Common/FundP4LogMacros.h"

int32 UP97_BatchActorHelper::AddTagToActors(const TArray<AActor*>& Actors, FName Tag)
{
	int32 Count = 0;
	for (AActor* A : Actors)
	{
		if (!A) { continue; }
		A->Tags.AddUnique(Tag);
		++Count;
	}
	P4_LOG("P97: AddTag %s 给 %d 个 Actor", *Tag.ToString(), Count);
	return Count;
}

int32 UP97_BatchActorHelper::PrefixActorTags(const TArray<AActor*>& Actors, FName Prefix)
{
	int32 Count = 0;
	int32 Index = 0;
	for (AActor* A : Actors)
	{
		if (!A) { continue; }
		const FName NewTag(*FString::Printf(TEXT("%s_%d"), *Prefix.ToString(), ++Index));
		A->Tags.AddUnique(NewTag);
		++Count;
	}
	return Count;
}

int32 UP97_BatchActorHelper::ToggleSimulatePhysics(const TArray<AActor*>& Actors, bool bEnable)
{
	int32 Count = 0;
	for (AActor* A : Actors)
	{
		if (!A) { continue; }
		// 遍历所有 StaticMeshComponent ：批量切换物理
		TArray<UStaticMeshComponent*> Meshes;
		A->GetComponents(Meshes);
		for (UStaticMeshComponent* M : Meshes)
		{
			if (!M) { continue; }
			M->SetMobility(bEnable ? EComponentMobility::Movable : EComponentMobility::Static);
			M->SetSimulatePhysics(bEnable);
			++Count;
		}
	}
	P4_LOG("P97: ToggleSimulatePhysics(%s) 影响 %d 个 Mesh",
		bEnable ? TEXT("ON") : TEXT("OFF"), Count);
	return Count;
}
