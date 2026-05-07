// P47-P50 - 资源加载综合 Helper
//
// P47 同步加载 ：LoadObject / StaticLoadObject / ConstructorHelpers (仅构造期)
// P48 异步加载 ：FStreamableManager::RequestAsyncLoad
// P49 AssetManager ：PrimaryAssetId 批量
// P50 Soft Reference 实战链路

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Components/ActorComponent.h"
#include "P47_50_AssetLoadingHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP48_OnAsyncLoaded, UObject*, Loaded);

UCLASS(ClassGroup = (P2), meta = (BlueprintSpawnableComponent))
class UETEST_API UP47_50_AssetLoadingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP47_50_AssetLoadingComponent();

	// P47 ：同步加载 ；阻塞 GameThread
	UFUNCTION(BlueprintCallable, Category = "P47|Sync")
	static UObject* SyncLoadByPath(const FSoftObjectPath& Path);

	// P48 ：异步加载（非阻塞）；完成后 OnAsyncLoaded 触发
	UFUNCTION(BlueprintCallable, Category = "P48|Async")
	void AsyncLoadByPath(const FSoftObjectPath& Path);

	UPROPERTY(BlueprintAssignable)
	FP48_OnAsyncLoaded OnAsyncLoaded;

	// P49 ：用 AssetManager 取某 PrimaryAssetType 的全部 Id
	UFUNCTION(BlueprintCallable, Category = "P49|AssetManager")
	static int32 GetPrimaryAssetIds(FName PrimaryAssetType, TArray<FPrimaryAssetId>& OutIds);

protected:
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	// 持有 Handle 防止 GC（SharedPtr 引用计数）
	TSharedPtr<FStreamableHandle> PendingHandle;

	void HandleStreamableComplete();
};
