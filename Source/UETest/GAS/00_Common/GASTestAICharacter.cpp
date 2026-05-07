// 公共 AI Character 实现

#include "GASTestAICharacter.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../A_Basics/A02_CombatSet.h"

AGASTestAICharacter::AGASTestAICharacter()
{
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// I43 - AI 用 Minimal ：只复制必要信息
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthSet = CreateDefaultSubobject<UHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UCombatSet>(TEXT("CombatSet"));
}

UAbilitySystemComponent* AGASTestAICharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASTestAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (AbilitySystemComponent)
	{
		// AI 的 ASC 在自身上，OwnerActor / AvatarActor 都是 this
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
