// P09 - 实现 IP09_Damageable 接口的示例 Actor
// 演示双类接口模式下 ：在 .h 中继承 IP09_Damageable

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P09_DamageableInterface.h"
#include "P09_DamageableActor.generated.h"

UCLASS()
class UETEST_API AP09_DamageableActor : public AActor, public IP09_Damageable
{
	GENERATED_BODY()

public:
	AP09_DamageableActor();

	UPROPERTY(VisibleAnywhere)
	float Health = 100.f;

	// 覆写 BlueprintNativeEvent 的 _Implementation
	virtual float TakeDamage_Implementation(float Amount) override;

	// 覆写纯 C++ 虚函数
	virtual bool IsDead() const override { return Health <= 0.f; }
};
