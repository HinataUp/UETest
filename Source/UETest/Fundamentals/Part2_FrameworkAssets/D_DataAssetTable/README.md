# D 章节 - DataAsset / DataTable / Chooser (P44-P46)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P44 | UPrimaryDataAsset | `P44_HeroDefinition.h/cpp` |
| P45 | UDataTable | `P45_WeaponDataTable.h/cpp` |
| P46 | Chooser Table（UE5.4+）| 无独立代码 ；本 README 给操作步骤 |

## P44 - PrimaryDataAsset

**与 UDataAsset 区别：**
- `UDataAsset` ：基础只读数据载体
- `UPrimaryDataAsset` ：override `GetPrimaryAssetId` 后 ，AssetManager 可扫描 / 异步加载 / Bundle

**编辑器创建实例：**
1. Content Browser 右键 → Miscellaneous → Data Asset
2. 选 `P44_HeroDefinition` 作为基类
3. 命名 `DA_Hero_Warrior` 等
4. 双击编辑数据

**配 AssetManager 扫描（DefaultGame.ini）：**
```ini
[/Script/Engine.AssetManagerSettings]
+PrimaryAssetTypesToScan=(PrimaryAssetType="Hero",
    AssetBaseClass=/Script/UETest.P44_HeroDefinition,
    bHasBlueprintClasses=False,
    bIsEditorOnly=False,
    Directories=((Path="/Game/Heroes")),
    Rules=(Priority=-1))
```

**P51 联动 - AssetBundles：**
```cpp
UPROPERTY(meta=(AssetBundles="Gameplay")) TSoftObjectPtr<USkeletalMesh> Mesh;
UPROPERTY(meta=(AssetBundles="UI"))       TSoftObjectPtr<UTexture2D>    Icon;
```
之后 `LoadPrimaryAsset(Id, {"UI"})` 只加载 UI Bundle 的资产。

## P45 - DataTable

**步骤：**
1. C++ 定义继承 `FTableRowBase` 的 RowStruct（已有 `FP45_WeaponData`）
2. Content Browser 新建 DataTable → 选 `P45_WeaponData` RowStruct
3. 编辑器内填表 / Reimport CSV

**查询：**
```cpp
FP45_WeaponData* Sword = Table->FindRow<FP45_WeaponData>(FName("Sword"), TEXT(""));
```

**CSV 导入格式：**
```
Name,DisplayName,Damage,AttackSpeed,Mesh
Sword,"长剑",50,1.2,/Game/Meshes/SM_Sword
Axe,"斧头",80,0.8,/Game/Meshes/SM_Axe
```

第一列必须是 `Name` ，作为 RowName 主键。

## P46 - Chooser Table（UE5.4+，仅笔记）

**前提：** `Plugins → Chooser`（Epic 内置）

**用途：** 数据驱动的条件选择 ；替代大量 if/else。

**编辑器步骤：**
1. Content Browser 新建 Chooser Table 资产
2. 配置 Inputs（条件列）：GameplayTag / Enum / Float Range
3. 配置 Outputs（结果列）：资产引用 / Class
4. 填行

**C++ 调用：**
```cpp
#include "ChooserFunctionLibrary.h"
UObject* Result = UChooserFunctionLibrary::EvaluateChooser(
    WorldContext, ChooserAsset, OutputType);
```

**典型场景：** 根据角色 Tag + 距离 + 任务阶段选择对话音效 / 动画 / 任务奖励。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → D_数据资产`
```
P44 PrimaryAssetId Type = "Hero"
P45 RowStruct 默认值
```

**手动 PIE：**
- P44 ：编辑器创建几个 DA → C++ 中 `UAssetManager::Get().GetPrimaryAssetIdList("Hero", Out)` 取到列表
- P45 ：手填表 → 用 `LookupWeapon(Table, "Sword", Out)` 验证查询

## 常见坑
- `UDataAsset`（不是 Primary）→ AssetManager 不扫描
- DefaultGame.ini 未配 Type → `GetPrimaryAssetIdList` 返回空
- DataTable 改 RowStruct 字段后忘了 Reimport → 旧数据丢失
- CSV 第一列不叫 Name → 导入失败
