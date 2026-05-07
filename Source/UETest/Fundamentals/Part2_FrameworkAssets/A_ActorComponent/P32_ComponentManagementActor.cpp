#include "P32_ComponentManagementActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../00_Common/FundP2LogMacros.h"

AP32_ComponentManagementActor::AP32_ComponentManagementActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 构造期创建 ：CreateDefaultSubobject 是唯一合法 API
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
}

UStaticMeshComponent* AP32_ComponentManagementActor::AddRuntimeMeshComponent()
{
	// 运行时创建组件 三步走 ：
	// 1) NewObject<T>(Outer) ：Outer 设为本 Actor ，决定生命周期
	UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this);
	if (!NewMesh) { return nullptr; }

	// 2) AttachToComponent ：附加到 RootComponent ；Component 才进入 Transform 层级
	NewMesh->AttachToComponent(
		RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// 3) RegisterComponent ：必须 ！否则 Tick / 渲染 / 物理都不工作
	NewMesh->RegisterComponent();

	P2_LOG("P32: 运行时新增 %s", *NewMesh->GetName());
	return NewMesh;
}

void AP32_ComponentManagementActor::DestroyComponent(UActorComponent* Comp)
{
	if (!Comp) { return; }

	// DestroyComponent ：完整反注册 + 销毁 ；不要直接 ConditionalBeginDestroy
	Comp->DestroyComponent();
	P2_LOG("P32: 销毁组件");
}
