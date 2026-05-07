#include "P35_LifecycleGameInstance.h"
#include "../00_Common/FundP2LogMacros.h"

void UP35_LifecycleGameInstance::Init()
{
	Super::Init();
	P2_LOG("P35: GameInstance::Init （进程启动唯一一次）");
}

void UP35_LifecycleGameInstance::Shutdown()
{
	P2_LOG("P35: GameInstance::Shutdown （进程退出）");
	Super::Shutdown();
}
