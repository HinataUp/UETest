// T12 - 普通 UObject 指针不能直接跨端
//
// 任务目标：踩"为什么 Client 拿到的是 nullptr"这个常见坑
//   * UObject 没有网络身份（NetGUID），Client 上不会自动出现
//   * 解决方案：
//       a) 改成 Actor   ：拥有网络复制能力（最直接）
//       b) ReplicatedSubObject ：让特定 UObject 跟着 Owner 一起复制（GAS 走的就是这条路，见 T26 ）
//       c) 用结构体复制内部数据，本端各自 NewObject 出"影子"

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T12_UObjectRefActor.generated.h"

// 演示用的"普通 UObject"，没特殊标记，无法被跨端解析
UCLASS()
class UETEST_API UT12_PlainInfoObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SomeValue = 0;
};

UCLASS()
class UETEST_API AT12_UObjectRefActor : public AActor
{
	GENERATED_BODY()

public:
	AT12_UObjectRefActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// 即便加了 Replicated ，Client 也无法解析这个引用
	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<UT12_PlainInfoObject> T12_InfoRef;

private:
	FTimerHandle T12_TickTimer;
	void T12_ServerCreateAndAssign();
	void T12_LogState();
};
