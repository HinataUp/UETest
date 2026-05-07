// P32 - Component 体系 ：构造期 / 运行时 创建与销毁
//
// 三种基类层级 ：
//   UActorComponent       ：无 Transform ；纯逻辑 ：HealthComponent / InventoryComponent
//   USceneComponent       ：有 Transform ；可附加到层级 ：SpringArm / Camera
//   UPrimitiveComponent   ：有渲染 / 碰撞 ：StaticMeshComponent / SphereComponent

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P32_ComponentManagementActor.generated.h"

class UActorComponent;
class UStaticMeshComponent;

UCLASS()
class UETEST_API AP32_ComponentManagementActor : public AActor
{
	GENERATED_BODY()

public:
	AP32_ComponentManagementActor();

	// 运行时新增组件 ：返回新创建的指针
	// NewObject + AttachToComponent + RegisterComponent 三步标准流程
	UFUNCTION(BlueprintCallable, Category = "P32")
	UStaticMeshComponent* AddRuntimeMeshComponent();

	// 运行时销毁组件
	UFUNCTION(BlueprintCallable, Category = "P32")
	void DestroyComponent(UActorComponent* Comp);

	// 构造期创建的 ：编辑器可见 ，作为 Root
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootScene;
};
