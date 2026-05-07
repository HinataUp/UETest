#include "P05_PropertySpecifierActor.h"
#include "Net/UnrealNetwork.h"

AP05_PropertySpecifierActor::AP05_PropertySpecifierActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;   // 让 Replicated 字段真正参与同步
}

void AP05_PropertySpecifierActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AP05_PropertySpecifierActor, J_Replicated);
}
