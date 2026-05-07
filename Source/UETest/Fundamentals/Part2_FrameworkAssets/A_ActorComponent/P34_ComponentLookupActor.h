// P34 - Component 通信模式
//
// 三种查找路径 ：
//   FindComponentByClass<T>            ：按类型 ；最常用
//   GetComponents(...)                 ：取全部组件
//   GetComponentsByTag(Class, Tag)     ：按 Component Tag 过滤（同类型多个时区分）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P34_ComponentLookupActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class UETEST_API AP34_ComponentLookupActor : public AActor
{
	GENERATED_BODY()

public:
	AP34_ComponentLookupActor();

	// 三个 MeshComponent ；用 Tag 区分用途
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> WeaponMesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> ShieldMesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> BodyMesh;

	// 按 Tag 找特定组件 ：经典用法
	UFUNCTION(BlueprintCallable, Category = "P34")
	UStaticMeshComponent* FindMeshByTag(FName Tag) const;
};
