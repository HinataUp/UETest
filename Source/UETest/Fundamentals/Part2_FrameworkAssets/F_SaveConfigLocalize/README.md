# F 章节 - Save / Config / Localization (P52-P54)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P52 | USaveGame 存档 | `P52_MySaveGame.h/cpp` |
| P53 | UCLASS(config=Game) + GConfig | `P53_MySettingsConfig.h/cpp` |
| P54 | FText / NSLOCTEXT / StringTable | `P54_LocalizationDemo.h/cpp` |

## P52 - SaveGame

```cpp
// 创建
UMySaveGame* Save = Cast<UMySaveGame>(
    UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
Save->PlayerName = TEXT("Hero");

// 存
UGameplayStatics::SaveGameToSlot(Save, TEXT("Slot1"), 0);

// 检查 + 读
if (UGameplayStatics::DoesSaveGameExist(TEXT("Slot1"), 0)) {
    UMySaveGame* L = Cast<UMySaveGame>(
        UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"), 0));
}
```

**关键：** `UPROPERTY(SaveGame)` 标记的字段才序列化。文件位于 `<Project>/Saved/SaveGames/<SlotName>.sav`。

**异步版：** `AsyncSaveGameToSlot` / `AsyncLoadGameFromSlot` ；用回调 ，不卡顿。

## P53 - Config

```cpp
UCLASS(Config = Game, DefaultConfig)
class UMySettings : public UObject {
    UPROPERTY(Config) float MasterVolume = 1.0f;
    UPROPERTY(Config) FString ServerIP   = TEXT("127.0.0.1");
};
// DefaultGame.ini ：
// [/Script/UETest.MySettings]
// MasterVolume=0.8
```

**读：** `GetDefault<UMySettings>()->MasterVolume`

**写：** `GetMutableDefault<UMySettings>()->SaveConfig()` 把当前值写回 ini

**底层：** `GConfig->GetFloat(TEXT("/Script/..."), TEXT("Key"), Out, GGameIni)` 直接读

## P54 - Localization

```cpp
#define LOCTEXT_NAMESPACE "MyGame"

FText Title = LOCTEXT("MainMenu_Title", "主菜单");

FText Damage = FText::Format(
    LOCTEXT("DamageLog", "对 {Target} 造成了 {Amount} 点伤害"),
    FFormatNamedArguments{
        {TEXT("Target"), FText::FromString(Name)},
        {TEXT("Amount"), FText::AsNumber(50)}
    });

#undef LOCTEXT_NAMESPACE
```

**StringTable 资产（推荐）：** Content Browser → Miscellaneous → String Table。表里给每个 ID 配多语言文案。代码用 `FText::FromStringTable(TableId, EntryKey)` 获取。

**翻译流程：** Project Settings → Localization Dashboard → Gather Text → Compile → 按目标语言导出 PO 文件给翻译。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → F_存档配置本地化`
```
P52 SaveGame 默认 Level=1 + InventoryItem 反射可达
P53 Config 类带 CLASS_Config flag
P54 FormatDamageText 含 Goblin / 50
```

**手动 PIE：**
- P52 ：调 `SaveToSlot` → 文件出现在 `Saved/SaveGames/Slot1.sav` → 重启 PIE → `LoadFromSlot` 数据一致
- P53 ：改 `DefaultGame.ini` → PIE → `GetDefault<UMySettings>()->MasterVolume` 读到新值
- P54 ：切换编辑器语言（Editor Preferences → Region & Language）→ FText 显示对应翻译

## 常见坑
- 字段忘加 `UPROPERTY(SaveGame)` → 不存盘；改了 RowStruct 字段后旧存档读取失败
- Config 类没加 `DefaultConfig` → SaveConfig 写到 Saved/Config/...（用户级）而不是项目 ini
- FText 用 `FText::FromString(TEXT("Hello"))` 持久化 UI → 失去翻译能力 ；该用 LOCTEXT
- StringTable 资产路径变更 → 引用的 ID 全失效
