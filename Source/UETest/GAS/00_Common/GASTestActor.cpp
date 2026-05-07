// 测试用极简 Actor 实现

#include "GASTestActor.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../A_Basics/A02_CombatSet.h"

AGASTestActor::AGASTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// 测试容器默认 Mixed ；想测 Minimal 时手动改
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<UHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UCombatSet>(TEXT("CombatSet"));
}

UAbilitySystemComponent* AGASTestActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASTestActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AGASTestActor::BeginPlay()
{
	Super::BeginPlay();
}
