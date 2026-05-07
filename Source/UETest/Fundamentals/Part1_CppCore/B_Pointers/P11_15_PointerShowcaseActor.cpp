#include "P11_15_PointerShowcaseActor.h"
#include "Components/StaticMeshComponent.h"
#include "../00_Common/FundamentalsLogMacros.h"

AP11_15_PointerShowcaseActor::AP11_15_PointerShowcaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// P11 ：CreateDefaultSubobject 返回的指针赋给 TObjectPtr ，自动转换
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

UStaticMesh* AP11_15_PointerShowcaseActor::LoadSoftMesh()
{
	// 软引用必须显式 Load ；LoadSynchronous 阻塞 ；可改 StreamableManager 异步
	UStaticMesh* Loaded = SoftMeshRef.LoadSynchronous();
	if (Loaded)
	{
		P1_LOG("P13: 软引用同步加载成功 = %s", *Loaded->GetName());
	}
	return Loaded;
}
