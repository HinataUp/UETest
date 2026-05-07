// P11-P15 - 指针类型综合演示
//
// P11 TObjectPtr      ：UE5 推荐的 UPROPERTY 指针 ；行为等价裸指针 + 运行时跟踪
// P12 TWeakObjectPtr  ：弱引用 ，不阻止 GC ；访问前 IsValid 检查
// P13 TSoftObjectPtr  ：软引用 ，存路径不立即加载
// P14 TSharedPtr/Unique：非 UObject 智能指针（标准 C++ 风格）
// P15 TSubclassOf     ：类型安全的 UClass 引用
//
// 一个 Actor 集中演示 ；自动化测试单测每种指针的行为

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "P11_15_PointerShowcaseActor.generated.h"

UCLASS()
class UETEST_API AP11_15_PointerShowcaseActor : public AActor
{
	GENERATED_BODY()

public:
	AP11_15_PointerShowcaseActor();

	// P11 ：UE5 推荐 ；UPROPERTY 中用 TObjectPtr
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// P12 ：弱引用 ；常用于"想引用但不想阻止 GC"的场景（缓存目标 / 临时引用）
	UPROPERTY()
	TWeakObjectPtr<AActor> WeakTarget;

	// P13 ：软引用 - 路径只存路径 ，运行时显式 Load 才占内存
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> SoftMeshRef;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> SoftClassRef;

	// P15 ：TSubclassOf - 编辑器中下拉只显示 AActor 子类
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	// 业务接口 ：验证软引用同步加载
	UFUNCTION(BlueprintCallable, Category = "P11-15")
	UStaticMesh* LoadSoftMesh();
};
