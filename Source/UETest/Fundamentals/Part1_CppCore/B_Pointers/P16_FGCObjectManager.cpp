#include "P16_FGCObjectManager.h"
#include "UObject/Object.h"
#include "UObject/ReferenceChainSearch.h"

void FP16_FGCObjectManager::Track(UObject* Obj)
{
	if (Obj)
	{
		ManagedObjects.Add(Obj);
	}
}

void FP16_FGCObjectManager::AddReferencedObjects(FReferenceCollector& Collector)
{
	// 关键 ：把 ManagedObjects 数组报告给 GC
	// GC 会把这些当作"根集"的一部分 ，不回收
	Collector.AddReferencedObjects(ManagedObjects);
}
