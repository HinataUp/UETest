# UE5 基础任务清单 · Part 2 ：Gameplay 框架 + 资产 + 子系统

## 总览
30 个任务覆盖 ：Actor/Component 架构、Gameplay 全家族、四种 Subsystem、Enhanced Input、GameplayTag、GameplayMessage、DataAsset/DataTable、资源加载（同步/异步/AssetManager/Bundle）、Save/Config/Localization、模块/插件/CVar/预处理、网络复制/Level Streaming。

## 章节地图

| 章节 | 任务 | 主题 | 文件夹 |
| --- | --- | --- | --- |
| **A** | P31-P34 | Actor / Component 架构 | `A_ActorComponent/` |
| **B** | P35-P40 | Gameplay 框架全家族 | `B_Framework/` |
| **C** | P41-P43 | Subsystem + Tag + Message | `C_Subsystems/` |
| **D** | P44-P46 | DataAsset / DataTable / Chooser | `D_DataAssetTable/` |
| **E** | P47-P51 | 资源加载（同步/异步/AssetManager/Bundle）| `E_AssetLoading/` |
| **F** | P52-P54 | Save / Config / Localization | `F_SaveConfigLocalize/` |
| **G** | P55-P58 | 模块 / 插件 / CVar / 预处理 | `G_ModulesPlugins/` |
| **H** | P59-P60 | 网络复制 + Level Streaming | `H_NetworkingLevel/` |
| 公共 | — | 日志宏 | `00_Common/` |
| 测试 | — | Spec | `Tests/` |

## 任务一览

### A - Actor / Component（4）
- P31 - Actor 完整生命周期（含 PostInitializeComponents）
- P32 - 构造期 vs 运行时 创建组件
- P33 - SpawnActor 三种方式 + ActorSpawnParameters
- P34 - FindComponentByClass / GetComponentsByTag

### B - Gameplay 框架（6）
- P35 - GameInstance 跨关卡持久
- P36 - GameMode（Server only） + GameState（Replicated）
- P37 - PlayerController + PlayerState
- P38 - Character + Possess 流程
- P39 - Enhanced Input（IA / IMC / TriggerEvent / Modifier）
- P40 - 完整初始化链路（GameInstance → ... → Character.BeginPlay）

### C - Subsystem + Message（3）
- P41 - 四种 Subsystem（Engine / GameInstance / World / LocalPlayer）
- P42 - GameplayTag 系统（与之前模块复用）
- P43 - GameplayMessageSubsystem（发布 / 订阅）

### D - 数据资产（3）
- P44 - UPrimaryDataAsset + AssetManager 扫描
- P45 - UDataTable + RowStruct + CSV 导入
- P46 - Chooser Table（UE5.4+）

### E - 资源加载（5）
- P47 - 同步加载（LoadObject / FObjectFinder）
- P48 - 异步加载（FStreamableManager）
- P49 - AssetManager + PrimaryAssetId 批量
- P50 - TSoftObjectPtr 实战
- P51 - AssetBundles + Chunk

### F - Save / Config / Localize（3）
- P52 - USaveGame 完整存读
- P53 - UCLASS(config=Game) + GConfig
- P54 - FText / NSLOCTEXT / StringTable

### G - 工程化（4）
- P55 - 模块系统（Build.cs / Target.cs）
- P56 - Plugin 架构（.uplugin / Loading Phase）
- P57 - CVar / Console Command / Exec
- P58 - 预处理 / 平台 / 构建配置 宏

### H - Network + Level（2）
- P59 - 网络复制综合（Replicated / OnRep / Server-Client-Multicast RPC）
- P60 - World Partition + LoadStreamLevel + ServerTravel

## 自动化测试

`UETest.Fundamentals.P2_框架与资产`
```
A_Actor与Component             ：4 用例
B_Gameplay框架                  ：4 用例
C_Subsystems与Tag               ：3 用例
D_数据资产                       ：2 用例
E_资源加载                       ：1 用例
F_存档配置本地化                 ：3 用例
G_工程化                        ：2 用例
H_网络与关卡                    ：1 用例
```

详见 `Tests/README.md`。

## 学习路径建议
1. 先 A 章节 ：建立 Actor/Component 基本生命周期手感
2. 再 B 章节 ：理清 Gameplay 框架家族关系（GameMode/State/PC/PS/Pawn 谁在哪端）
3. C / D / E 并行 ：分别是"系统抽象 / 数据驱动 / 资源加载"三个独立维度
4. F 章节作为"持久化"知识点
5. G 章节按需 ：项目工程化时回看
6. H 章节是 Part 1 + 网络同步模块的应用收尾

## 与其他模块的边界

| 知识点 | 详尽覆盖在 |
| --- | --- |
| 网络复制 30 个 task | `Source/UETest/NetworkReplication/` |
| GAS 47 个 task | `Source/UETest/GAS/` |
| Shader / 渲染 15 个 task | `Source/UETest/Rendering/` |
| C++ 核心 30 个 task | `Source/UETest/Fundamentals/Part1_CppCore/` |

本 Part 2 在与上述模块重叠时 ：只演示一个综合 Actor / Helper ，避免重复。

## 前置依赖
- 无需修改 `Build.cs` ：所需依赖（Core / CoreUObject / Engine / EnhancedInput）已在主模块
- `GameplayMessageSubsystem` 来自插件 Gameplay Message Router ：用 P43 时手动 Project Settings → Plugins 启用
- `Chooser` 插件（P46）：Epic 内置 ；用 P46 时启用
