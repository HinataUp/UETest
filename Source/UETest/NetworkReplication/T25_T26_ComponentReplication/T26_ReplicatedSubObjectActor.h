// T26 - ReplicatedSubObject（让普通 UObject 也能跨端同步）
//
// 任务目标：解决 T12 的痛点 —— 普通 UObject 无法跨端
// UE5 推荐 API（5.1+）：AddReplicatedSubObject / RemoveReplicatedSubObject
// 旧 API（仍可用）  ：override ReplicateSubobjects
//
// 应用：GAS 的 AbilitySystemComponent 用此机制同步 AttributeSet 和 GameplayAbility

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T26_ReplicatedSubObjectActor.generated.h"

// 关键：必须 override IsSupportedForNetworking() 返回 true
UCLASS()
class UETEST_API UT26_RepSubObject : public UObject
{
	GENERATED_BODY()
public:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UObject 自己也要支持 RepProps
	UPROPERTY(Replicated)
	int32 SubObjValue = 0;
};

UCLASS()
class UETEST_API AT26_ReplicatedSubObjectActor : public AActor
{
	GENERATED_BODY()

public:
	AT26_ReplicatedSubObjectActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UE5 推荐写法：开启基于注册表的子对象复制 ，配合 AddReplicatedSubObject 使用
	// 启用后无需 override ReplicateSubobjects
	virtual bool IsSupportedForNetworking() const override { return true; }

protected:
	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<UT26_RepSubObject> T26_SubObj;

private:
	FTimerHandle T26_TickTimer;
	void T26_ServerInitAndTick();
	void T26_ServerTickValue();
};
