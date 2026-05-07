// T10 - 复制 Actor 指针（关卡中已存在的 Actor）
//
// 任务目标：理解"指针复制不是地址，而是网络对象引用"
//   * 关卡 Actor 在 Client 加载时本地创建，因此引用容易解析成功
//   * 引用的目标 Actor 必须在 Client 上"存在"才能被解析
//
// 写法对比：
//   写法1：UPROPERTY(Replicated) AActor* Ptr ;
//   写法2：UPROPERTY(Replicated) TObjectPtr<AActor> Ptr ;  （UE5 推荐，行为等价）
//   写法3：TWeakObjectPtr<AActor> ：不参与复制，仅本地弱引用，不能复制

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T10_LevelActorRefActor.generated.h"

UCLASS()
class UETEST_API AT10_LevelActorRefActor : public AActor
{
	GENERATED_BODY()

public:
	AT10_LevelActorRefActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Editor 中可在 Details 面板拖入关卡里的目标 Actor
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> T10_TargetInLevel;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T10_TargetActor, VisibleAnywhere)
	TObjectPtr<AActor> T10_TargetActor;

	UFUNCTION()
	void OnRep_T10_TargetActor();

private:
	FTimerHandle T10_TickTimer;
	void T10_ServerAssignTarget();
};
