#include "F33_HitFireCueNotify.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UCueNotify_HitFire::UCueNotify_HitFire()
{
	GameplayCueTag = MyGameplayTags::TAG_GameplayCue_Hit_Fire;
}

bool UCueNotify_HitFire::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	GAS_LOG("【BurstCue】HitFire OnExecute Target=%s",
		MyTarget ? *MyTarget->GetName() : TEXT("null"));
	// 实战：UNiagaraFunctionLibrary::SpawnSystemAtLocation + 播放 SFX
	return true;
}
