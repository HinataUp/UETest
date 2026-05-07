#include "P41_AllSubsystems.h"
#include "../00_Common/FundP2LogMacros.h"

void UP41_EngineSubsystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	P2_LOG("P41: EngineSubsystem::Initialize （进程级）");
}
void UP41_EngineSubsystem::Deinitialize()
{
	P2_LOG("P41: EngineSubsystem::Deinitialize");
	Super::Deinitialize();
}

void UP41_GameInstanceSubsystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	P2_LOG("P41: GameInstanceSubsystem::Initialize （进程内每个 GameInstance 一次）");
}
void UP41_GameInstanceSubsystem::Deinitialize()
{
	P2_LOG("P41: GameInstanceSubsystem::Deinitialize");
	Super::Deinitialize();
}

void UP41_WorldSubsystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	P2_LOG("P41: WorldSubsystem::Initialize （每个 World 一个）");
}
void UP41_WorldSubsystem::Deinitialize()
{
	P2_LOG("P41: WorldSubsystem::Deinitialize");
	Super::Deinitialize();
}
bool UP41_WorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	// 只在 Game / PIE 创建 ；编辑器 World 不需要
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UP41_LocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	P2_LOG("P41: LocalPlayerSubsystem::Initialize");
}
void UP41_LocalPlayerSubsystem::Deinitialize()
{
	P2_LOG("P41: LocalPlayerSubsystem::Deinitialize");
	Super::Deinitialize();
}
