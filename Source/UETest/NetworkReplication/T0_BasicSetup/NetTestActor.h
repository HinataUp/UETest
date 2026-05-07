// T0 - 网络同步测试基础 Actor
// 任务：搭建一个最小可复制的 Actor，作为后续所有 task 的载体
// 验收：BeginPlay 在三端都打日志，并能区分 Server / Client 身份
//
// 关键点：
// 1. bReplicates = true ：Actor 参与网络复制的总开关
// 2. 拥有 StaticMeshComponent ，方便在 Viewport 看见
// 3. 此 Actor 没有 Owner ，因此在客户端是 SimulatedProxy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetTestActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class UETEST_API ANetTestActor : public AActor
{
	GENERATED_BODY()

public:
	ANetTestActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// 可见网格，方便在场景中识别
	// 命名加 NT_（NetTest）前缀，避免与 UE 内置 Mesh 命名冲突
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> NT_MeshComp;
};
