#include "P59_NetworkBasicsActor.h"
#include "Net/UnrealNetwork.h"
#include "../00_Common/FundP2LogMacros.h"

AP59_NetworkBasicsActor::AP59_NetworkBasicsActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AP59_NetworkBasicsActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AP59_NetworkBasicsActor, Health);
}

void AP59_NetworkBasicsActor::OnRep_Health()
{
	P2_LOG("P59: OnRep Health=%d", Health);
}

bool AP59_NetworkBasicsActor::ServerRequestDamage_Validate(int32 Amount)
{
	return Amount >= 0 && Amount <= 1000;
}

void AP59_NetworkBasicsActor::ServerRequestDamage_Implementation(int32 Amount)
{
	Health = FMath::Max(0, Health - Amount);
	P2_LOG("P59: Server 处理伤害 ：剩余 Health=%d", Health);
}

void AP59_NetworkBasicsActor::ClientShowMessage_Implementation(const FString& Msg)
{
	P2_LOG("P59: ClientRPC 收到消息 ：%s", *Msg);
}

void AP59_NetworkBasicsActor::MulticastPlayHitFX_Implementation(FVector Location)
{
	P2_LOG("P59: Multicast HitFX @ (%.1f,%.1f,%.1f)", Location.X, Location.Y, Location.Z);
}
