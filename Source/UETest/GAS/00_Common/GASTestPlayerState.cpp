// 公共测试 PlayerState 实现

#include "GASTestPlayerState.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../A_Basics/A02_CombatSet.h"

AGASTestPlayerState::AGASTestPlayerState()
{
	// PlayerState 默认就 bReplicates=true ，无需重设

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// I43 Mixed ：Owner 收完整 GE 信息，Simulated 只收必要 Tag/属性
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AttributeSet 作为 DefaultSubobject 创建在 PlayerState 上
	HealthSet = CreateDefaultSubobject<UHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UCombatSet>(TEXT("CombatSet"));

	// PlayerState Tick 频率较低 ，避免和 Pawn 重复 Tick
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AGASTestPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASTestPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	}
}
