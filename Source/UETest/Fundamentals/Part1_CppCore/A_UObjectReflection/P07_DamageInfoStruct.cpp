#include "P07_DamageInfoStruct.h"

bool FP07_DamageInfo::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// Amount 用 float 直传 ；实际项目可量化压缩
	Ar << Amount;

	// FGameplayTag 自带 NetSerialize
	DamageType.NetSerialize(Ar, Map, bOutSuccess);

	// Instigator 是 WeakObjectPtr ；用 PackageMap 序列化
	UObject* Inst = Instigator.Get();
	Map->SerializeObject(Ar, AActor::StaticClass(), Inst);
	if (Ar.IsLoading())
	{
		Instigator = Cast<AActor>(Inst);
	}

	bOutSuccess = true;
	return true;
}
