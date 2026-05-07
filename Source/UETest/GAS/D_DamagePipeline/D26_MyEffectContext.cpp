#include "D26_MyEffectContext.h"

bool FMyGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	// 先序列化父类字段
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
	// 再序列化自定义字段
	Ar << bIsCrit;
	bOutSuccess = true;
	return true;
}
