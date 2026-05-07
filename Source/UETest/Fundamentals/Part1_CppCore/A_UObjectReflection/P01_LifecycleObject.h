// P01 - UObject 生命周期 ：NewObject / SpawnActor / CreateDefaultSubobject
//
// 三种创建方式的适用场景 ：
//   * NewObject<T>(Outer)         ：所有非 Actor 的 UObject
//   * SpawnActor<T>(Class, ...)   ：专用于 Actor （内部仍调 NewObject + 注册到 World）
//   * CreateDefaultSubobject<T>   ：只能在 构造函数 内调用 ；运行时创建组件用 NewObject + RegisterComponent
//
// 销毁 ：
//   * Actor      ：Destroy()
//   * 普通 UObject ：不能 Destroy ，靠 GC 回收 ；可显式 MarkAsGarbage()

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "P01_LifecycleObject.generated.h"

// 普通 UObject ：演示 NewObject / MarkAsGarbage
UCLASS()
class UETEST_API UP01_PlainObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 TestValue = 0;
};

// 演示 CreateDefaultSubobject 的组件
UCLASS(ClassGroup = (P1), meta = (BlueprintSpawnableComponent))
class UETEST_API UP01_DemoComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UP01_DemoComponent();
};

// 演示 SpawnActor + 构造期 CreateDefaultSubobject
UCLASS()
class UETEST_API AP01_LifecycleActor : public AActor
{
	GENERATED_BODY()

public:
	AP01_LifecycleActor();

	// 子组件必须在 构造函数 中 CreateDefaultSubobject ；运行时不能用此 API
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UP01_DemoComponent> DefaultSubobjectComp;

	// 演示运行时新增 Component 的标准做法 ：NewObject + RegisterComponent
	UFUNCTION(BlueprintCallable, Category = "P01")
	UP01_DemoComponent* AddRuntimeComponent();
};
