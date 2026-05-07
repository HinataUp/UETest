#include "D26_MyAbilitySystemGlobals.h"
#include "D26_MyEffectContext.h"

FGameplayEffectContext* UMyAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	// 全项目所有 EffectContext 都用我们的子类
	return new FMyGameplayEffectContext();
}
