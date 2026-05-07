# E 章节 - 资源加载全覆盖 (P47-P51)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P47 | 同步加载 | `P47_50_AssetLoadingHelper`（合并）|
| P48 | 异步加载（FStreamableManager）| 同上 |
| P49 | UAssetManager + PrimaryAssetId | 同上 |
| P50 | TSoftObjectPtr 实战链路 | 同上 |
| P51 | Asset Bundle / Chunk 划分 | 见本 README"P51 配置"|

## 速查

```cpp
// P47 同步 ：阻塞 GameThread
UObject* O = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/SM_Cube"));
UObject* O2 = SoftPath.TryLoad();

// 仅构造期
static ConstructorHelpers::FObjectFinder<UStaticMesh> F(TEXT("/Game/Meshes/SM_Cube"));
if (F.Succeeded()) Mesh = F.Object;

// P48 异步 ：非阻塞 + 回调
FStreamableManager& Mgr = UAssetManager::GetStreamableManager();
TSharedPtr<FStreamableHandle> H = Mgr.RequestAsyncLoad(
    Path,
    FStreamableDelegate::CreateUObject(this, &::OnLoaded),
    FStreamableManager::AsyncLoadHighPriority);

// P49 AssetManager 批量
UAssetManager::Get().GetPrimaryAssetIdList("Hero", OutIds);
UAssetManager::Get().LoadPrimaryAsset(Id, {"UI"}, FStreamableDelegate::CreateLambda(...));

// P50 Soft 实战链路
UPROPERTY(EditAnywhere) TSoftObjectPtr<UTexture2D> Icon;
if (!Icon.IsNull()) {
    UTexture2D* T = Icon.LoadSynchronous();   // 或 RequestAsyncLoad
}
```

## P51 - Asset Bundle 配置（已在 P44 演示）

```cpp
UPROPERTY(meta=(AssetBundles="Gameplay")) TSoftObjectPtr<USkeletalMesh> Mesh;
UPROPERTY(meta=(AssetBundles="UI"))       TSoftObjectPtr<UTexture2D>    Icon;
```

加载时只取部分 Bundle ：
```cpp
UAssetManager::Get().LoadPrimaryAsset(Id, /*BundleNames=*/{"UI"}, ...);
// 此时 Icon 会被加载 ，Mesh 不加载
```

**Chunk 配置（Project Settings → Packaging）：**
- 给 PrimaryAssetType 分配 ChunkId ：例 Hero -> Chunk 1, Map -> Chunk 2
- 打包时生成多个 .pak 文件 ；按需下载实现 DLC / 增量更新

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → E_资源加载`
```
P47 SyncLoad 空路径返回 nullptr
```

**手动 PIE：**
- P47 ：BP / C++ 调 `SyncLoadByPath(/Game/Meshes/SM_Cube)` → 返回非空 ；首次加载有卡顿（同步阻塞）
- P48 ：调 `AsyncLoadByPath` → 立即返回 ；OnAsyncLoaded 委托稍后触发 ；同时间游戏不卡
- P49 ：编辑器创建 5 个 HeroDefinition → 配 DefaultGame.ini → C++ `GetPrimaryAssetIds("Hero", Out)` 返回 5

## 常见坑
- `ConstructorHelpers::FObjectFinder` 在运行时调用 → 崩；只能构造期
- 同步加载大资产 → 卡顿；改用异步 + 加载界面
- 异步加载的 `FStreamableHandle` 不持有 → 加载结果可能被 GC
- 不调 `CancelHandle` 直接销毁组件 → 异步回调进入已销毁对象；本示例 EndPlay 中处理
- 路径错（SM_Cube vs SM_Cube_C） → 失败；C++ 中类的路径是 `/Game/.../SM_Cube.SM_Cube`
