// T13 - 复制 Component 指针
//
// 任务目标：DefaultSubobject Component 在两端都有稳定 NetGUID ，可被复制定位
// 与 T12 区别：
//   * Component 是 Actor 的 DefaultSubobject ，Client 加载 Actor 时会同时构造同名 Component
//   * UObject 没这个待遇，所以 T12 失败
//
// 提示：运行时 AddComponent 创建的 Component 默认无网络身份；要复制需走 SetIsReplicated + 注册流程，
// 大部分场景仍优先做成 DefaultSubobject 。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T13_ComponentRefActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class UETEST_API AT13_ComponentRefActor : public AActor
{
	GENERATED_BODY()

public:
	AT13_ComponentRefActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// 构造期 CreateDefaultSubobject ：两端都有同一身份的 Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> T13_MeshComp;

	// 用 Replicated 字段引用上面那个 Component
	UPROPERTY(ReplicatedUsing = OnRep_T13_RepMeshRef, VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> T13_RepMeshRef;

	UFUNCTION()
	void OnRep_T13_RepMeshRef();

private:
	FTimerHandle T13_TickTimer;
	void T13_ServerAssignSelfMesh();
};
