#include "F33_BurningCueNotify.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

ACueNotify_Burning::ACueNotify_Burning()
{
	GameplayCueTag = MyGameplayTags::TAG_GameplayCue_Buff_Burning;
}

bool ACueNotify_Burning::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	GAS_LOG("【ActorCue】Burning OnActive Target=%s",
		MyTarget ? *MyTarget->GetName() : TEXT("null"));
	// 实战：SpawnNiagaraSystem 持续燃烧粒子，挂在 Target 身上
	return true;
}

bool ACueNotify_Burning::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// 中途加入的观察者会触发此回调；正常已激活的 Cue 不重复触发
	GAS_LOG("【ActorCue】Burning WhileActive ：让中途加入的 Client 也能看到");
	return true;
}

bool ACueNotify_Burning::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	GAS_LOG("【ActorCue】Burning OnRemove ：清理粒子");
	return true;
}
