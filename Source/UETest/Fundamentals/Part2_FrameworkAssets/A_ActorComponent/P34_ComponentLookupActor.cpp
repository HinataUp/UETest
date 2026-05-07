#include "P34_ComponentLookupActor.h"
#include "Components/StaticMeshComponent.h"

AP34_ComponentLookupActor::AP34_ComponentLookupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->ComponentTags.Add(TEXT("Weapon"));
	RootComponent = WeaponMesh;

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->ComponentTags.Add(TEXT("Shield"));
	ShieldMesh->SetupAttachment(RootComponent);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->ComponentTags.Add(TEXT("Body"));
	BodyMesh->SetupAttachment(RootComponent);
}

UStaticMeshComponent* AP34_ComponentLookupActor::FindMeshByTag(FName Tag) const
{
	// GetComponentsByTag ：按 ComponentTags 过滤同类型组件
	TArray<UActorComponent*> Found;
	GetComponents(UStaticMeshComponent::StaticClass(), Found);
	for (UActorComponent* C : Found)
	{
		if (C->ComponentHasTag(Tag))
		{
			return Cast<UStaticMeshComponent>(C);
		}
	}
	return nullptr;
}
