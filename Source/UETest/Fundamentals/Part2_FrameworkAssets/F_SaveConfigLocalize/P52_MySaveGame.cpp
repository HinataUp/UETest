#include "P52_MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "../00_Common/FundP2LogMacros.h"

bool UP52_SaveGameHelper::SaveToSlot(UP52_MySaveGame* SaveData, const FString& SlotName)
{
	if (!SaveData) { return false; }
	const bool bOK = UGameplayStatics::SaveGameToSlot(SaveData, SlotName, /*UserIndex=*/0);
	P2_LOG("P52: 存档 %s -> %s", *SlotName, bOK ? TEXT("成功") : TEXT("失败"));
	return bOK;
}

UP52_MySaveGame* UP52_SaveGameHelper::LoadFromSlot(const FString& SlotName)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) { return nullptr; }
	return Cast<UP52_MySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
}
