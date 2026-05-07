# A 章节 - UMG / Slate / UI (P61-P69)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P61 | UUserWidget + BindWidget | `P61_HUDWidget.h/cpp` |
| P62 | 事件绑定 + 数据驱动 | `P62_MenuWidget.h/cpp` |
| P63 | UListView 数据驱动列表 | `P63_InventoryListView.h/cpp` |
| P64 | Common UI Plugin | 见下方"P64 步骤"|
| P65 | Slate SCompoundWidget | `P65_SlatePanel.h/cpp` |
| P66 | Slate 布局 | 见下方"P66 笔记"|
| P67 | FSlateStyleSet | 见下方"P67 笔记"|
| P68 | AHUD + Canvas | `P68_GameHUD.h/cpp` |
| P69 | Input Mode 管理 | `P69_InputModeHelper.h/cpp` |

## P61 - UUserWidget + BindWidget

```cpp
UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> HealthText;
UPROPERTY(meta=(BindWidgetOptional)) TObjectPtr<UImage> Portrait;   // 可缺
```

**编辑器：**
1. Content Browser → User Interface → Widget Blueprint
2. Reparent to `P61_HUDWidget`
3. Designer 加 TextBlock 命名 `HealthText` / ProgressBar 命名 `HealthBar`
4. C++ 中 `CreateWidget<UP61_HUDWidget>(PC, WidgetClass)` + `AddToViewport()`

**关键：** BindWidget 字段名 / 类型 必须与 BP 中完全一致 ；不一致编译警告。

## P62 - 事件绑定（推荐手动模式）

```cpp
StartButton->OnClicked.AddDynamic(this, &UP62_MenuWidget::OnStartClicked);
```

**Property Binding 反模式：** 编辑器中"Bind"下拉直接绑函数，每帧调用 ；高频字段慎用。

**手动更新模式：** 数据变化时调 `Widget->UpdateData(...)` ；只在变化时刷 UI。

## P63 - ListView

**虚拟化：** `SetListItems(N 条数据)` 后只创建可见窗口数量的 EntryWidget ；滚动时复用控件并调 `NativeOnListItemObjectSet(NewData)` 重新填充。

```cpp
TArray<UObject*> Items = ...;
ListView->SetListItems(Items);
```

## P64 - Common UI（Plugin）

**启用：** Project Settings → Plugins → Common UI

**核心类：**
- `UCommonActivatableWidget` ：基础"可激活"Widget ；NativeOnActivated/Deactivated
- `UCommonActivatableWidgetStack` ：栈式管理 ；Push/Pop 自动激活栈顶
- `UCommonInputSubsystem` ：输入路由
- `UCommonUIInputData` ：默认 Back / Confirm 输入

**步骤：**
1. 主菜单继承 `UCommonActivatableWidget`
2. `WidgetStack->AddWidget<USettingsWidget>()` 推入设置界面
3. 按 ESC → Back Action 自动 Pop 栈顶

## P65 - Slate SCompoundWidget

```cpp
SLATE_BEGIN_ARGS(SP65_Panel) : _Title(NSLOCTEXT("P65", "Default", "标题")) {}
    SLATE_ARGUMENT(FText, Title)
    SLATE_EVENT(FOnClicked, OnButtonClicked)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs) {
    ChildSlot[ SNew(SVerticalBox) + ... ];
}
```

**用法：** `SAssignNew(MyPanel, SP65_Panel).Title(...).OnButtonClicked(...)`

## P66 - Slate 布局笔记

| 控件 | 作用 |
| --- | --- |
| `SVerticalBox / SHorizontalBox` | 线性布局 |
| `SOverlay` | 叠加（背景 + 文字）|
| `SScrollBox` | 滚动区 |
| `SSplitter` | 可拖动分隔栏 |
| `SBox` | 固定宽高 / Padding 容器 |

**Slot 修饰：**
```cpp
+ SVerticalBox::Slot()
    .AutoHeight()           // 自适应高度
    .FillHeight(0.5f)       // 占父容器一半
    .Padding(8.f)
    .HAlign(HAlign_Center)
    .VAlign(VAlign_Top)
```

## P67 - FSlateStyleSet 笔记

```cpp
FSlateStyleSet StyleSet("MyStyle");
StyleSet.Set("Button.Background", new FSlateColorBrush(FLinearColor::Blue));
StyleSet.Set("Title.Font", FSlateFontInfo(...));
FSlateStyleRegistry::RegisterSlateStyle(StyleSet);
```

实战 ：定义按钮在 Normal/Hovered/Pressed 三态颜色。

## P68 - AHUD::DrawHUD

```cpp
virtual void DrawHUD() override {
    Super::DrawHUD();
    DrawLine(...);  // 屏幕坐标
    Canvas->DrawText(...);
    Canvas->Project(WorldLocation);   // 3D -> 2D 屏幕坐标
}
```

**配置：** GameMode 的 `HUDClass = AP68_GameHUD::StaticClass()`

## P69 - Input Mode

| 模式 | 用途 |
| --- | --- |
| `FInputModeGameOnly` | 纯游戏控制 |
| `FInputModeUIOnly` | 纯 UI 操作 |
| `FInputModeGameAndUI` | UI 优先 + 游戏接收剩余输入 |

```cpp
UP69_InputModeHelper::SetUIOnly(PC, MenuWidget);   // 打开菜单
UP69_InputModeHelper::SetGameOnly(PC);             // 关闭菜单
```

## 测试方法

**自动化测试：** `UETest.Fundamentals.P3_UI_AI_动画 → A_UI`
```
P61 BindWidget 字段反射可达
P62 OnStartGameRequested 委托可达
P63 Slot Widget 实现 IUserObjectListEntry
P68 GameHUD 默认配置
```

**手动 PIE：** 见各 task 编辑器步骤。

## 常见坑
- BindWidget 名字 / 类型不一致 → Compile 警告但运行时为 nullptr，访问崩
- 大量 AddDynamic 但忘 RemoveDynamic ：Widget 销毁时按钮也销毁，不致命；但保持对称习惯
- ListView 用 `AddItem` 频繁触发刷新 → 用 `SetListItems` 一次性更新
- Common UI Stack 嵌套时 Back 路由错乱 → 检查 Activatable 的 Input Mode
