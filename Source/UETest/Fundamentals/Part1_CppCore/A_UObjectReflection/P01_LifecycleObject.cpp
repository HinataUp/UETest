#include "P01_LifecycleObject.h"
#include "../00_Common/FundamentalsLogMacros.h"

UP01_DemoComponent::UP01_DemoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AP01_LifecycleActor::AP01_LifecycleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 唯一合法时机 ：构造函数中 CreateDefaultSubobject
	DefaultSubobjectComp = CreateDefaultSubobject<UP01_DemoComponent>(TEXT("DefaultSubobjectComp"));
}

UP01_DemoComponent* AP01_LifecycleActor::AddRuntimeComponent()
{
	// 运行时新增 Component 的两步法 ：
	// 1) NewObject 创建实例 ，Outer 设为本 Actor
	// 2) RegisterComponent 接入 Tick / 渲染 / 物理 等系统
	UP01_DemoComponent* NewComp = NewObject<UP01_DemoComponent>(this);
	if (NewComp)
	{
		NewComp->RegisterComponent();
		P1_LOG("P01: 运行时新增组件 %s", *NewComp->GetName());
	}
	return NewComp;
}
