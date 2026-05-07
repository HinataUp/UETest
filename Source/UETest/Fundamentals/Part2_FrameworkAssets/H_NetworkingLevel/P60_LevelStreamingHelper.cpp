#include "P60_LevelStreamingHelper.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../00_Common/FundP2LogMacros.h"

void UP60_LevelStreamingHelper::LoadStreamLevelByName(
	const UObject* WorldContext, FName LevelName,
	bool bMakeVisibleAfterLoad, bool bShouldBlock)
{
	if (!WorldContext) { return; }
	FLatentActionInfo Info;
	Info.CallbackTarget = const_cast<UObject*>(WorldContext);
	UGameplayStatics::LoadStreamLevel(
		WorldContext, LevelName, bMakeVisibleAfterLoad, bShouldBlock, Info);
	P2_LOG("P60: LoadStreamLevel %s", *LevelName.ToString());
}

void UP60_LevelStreamingHelper::UnloadStreamLevelByName(
	const UObject* WorldContext, FName LevelName, bool bShouldBlock)
{
	if (!WorldContext) { return; }
	FLatentActionInfo Info;
	Info.CallbackTarget = const_cast<UObject*>(WorldContext);
	UGameplayStatics::UnloadStreamLevel(WorldContext, LevelName, Info, bShouldBlock);
	P2_LOG("P60: UnloadStreamLevel %s", *LevelName.ToString());
}

void UP60_LevelStreamingHelper::ServerTravelToMap(const UObject* WorldContext, const FString& MapPath)
{
	if (!WorldContext) { return; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return; }
	// ServerTravel ：所有客户端跟随
	// 想 Seamless 切换 ：World->GetGameMode()->bUseSeamlessTravel = true ；
	// 还要在 Project Settings 配置 TransitionMap
	World->ServerTravel(MapPath);
	P2_LOG("P60: ServerTravel -> %s", *MapPath);
}
