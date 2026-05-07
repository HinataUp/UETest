// T25 - Replicated Component 实现

#include "T25_RepCombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "../Common/NetTestLogMacros.h"

UT25_RepCombatComponent::UT25_RepCombatComponent()
	: T25_CompValue(0)
{
	PrimaryComponentTick.bCanEverTick = false;

	// UE5 推荐写法：在构造期开启复制
	// 等价于 BP 中 Component Replicates 勾选
	SetIsReplicatedByDefault(true);
}

void UT25_RepCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Component 自己也要写 GetLifetimeReplicatedProps ，注册自身字段
	DOREPLIFETIME(UT25_RepCombatComponent, T25_CompValue);
}

void UT25_RepCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		GetOwner()->GetWorldTimerManager().SetTimer(
			T25_TickTimer, this, &UT25_RepCombatComponent::T25_ServerTick,
			2.0f, true);
	}
}

void UT25_RepCombatComponent::T25_ServerTick()
{
	++T25_CompValue;
	UE_LOG(LogTemp, Warning, TEXT("[%s|Comp] ServerTick CompValue=%d"),
		*GetOwner()->GetName(), T25_CompValue);
}

void UT25_RepCombatComponent::OnRep_T25_CompValue()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s|Comp] OnRep CompValue=%d"),
		*GetOwner()->GetName(), T25_CompValue);
}

// ----------------- Actor 端 -----------------

AT25_RepComponentActor::AT25_RepComponentActor()
{
	bReplicates = true;
	T25_CombatComp = CreateDefaultSubobject<UT25_RepCombatComponent>(TEXT("T25_CombatComp"));
}
