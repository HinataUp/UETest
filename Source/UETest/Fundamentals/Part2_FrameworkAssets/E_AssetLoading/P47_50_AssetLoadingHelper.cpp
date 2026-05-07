#include "P47_50_AssetLoadingHelper.h"
#include "Engine/AssetManager.h"
#include "../00_Common/FundP2LogMacros.h"

UP47_50_AssetLoadingComponent::UP47_50_AssetLoadingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UObject* UP47_50_AssetLoadingComponent::SyncLoadByPath(const FSoftObjectPath& Path)
{
	if (Path.IsNull()) { return nullptr; }
	// 同步加载 ：阻塞 GameThread ；大资产慎用
	UObject* Loaded = Path.TryLoad();
	P2_LOG("P47: SyncLoad %s -> %s",
		*Path.ToString(),
		Loaded ? *Loaded->GetName() : TEXT("null"));
	return Loaded;
}

void UP47_50_AssetLoadingComponent::AsyncLoadByPath(const FSoftObjectPath& Path)
{
	if (Path.IsNull()) { return; }
	if (!UAssetManager::IsInitialized()) { return; }

	FStreamableManager& Mgr = UAssetManager::GetStreamableManager();
	PendingHandle = Mgr.RequestAsyncLoad(
		Path,
		FStreamableDelegate::CreateUObject(this, &UP47_50_AssetLoadingComponent::HandleStreamableComplete),
		FStreamableManager::AsyncLoadHighPriority);

	P2_LOG("P48: 异步加载已发起 ：%s", *Path.ToString());
}

void UP47_50_AssetLoadingComponent::HandleStreamableComplete()
{
	if (!PendingHandle.IsValid()) { return; }
	UObject* Loaded = PendingHandle->GetLoadedAsset();
	P2_LOG("P48: 异步加载完成 ：%s",
		Loaded ? *Loaded->GetName() : TEXT("null"));
	OnAsyncLoaded.Broadcast(Loaded);
	PendingHandle.Reset();
}

void UP47_50_AssetLoadingComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	// EndPlay ：取消未完成的异步加载 ，避免回调进入已销毁组件
	if (PendingHandle.IsValid())
	{
		PendingHandle->CancelHandle();
		PendingHandle.Reset();
	}
	Super::EndPlay(Reason);
}

int32 UP47_50_AssetLoadingComponent::GetPrimaryAssetIds(FName PrimaryAssetType, TArray<FPrimaryAssetId>& OutIds)
{
	OutIds.Reset();
	if (!UAssetManager::IsInitialized()) { return 0; }

	UAssetManager& AM = UAssetManager::Get();
	AM.GetPrimaryAssetIdList(PrimaryAssetType, OutIds);
	return OutIds.Num();
}
