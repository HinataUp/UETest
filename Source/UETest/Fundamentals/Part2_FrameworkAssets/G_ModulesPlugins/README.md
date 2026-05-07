# G 章节 - 模块 / 插件 / CVar / 预处理 (P55-P58)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P55 | 模块系统（Build.cs / Target.cs）| 见本 README |
| P56 | Plugin 架构（.uplugin）| 见本 README |
| P57 | CVar / Console Command / Exec | `P57_CVarCommandDemo.h/cpp` |
| P58 | 预处理 / 平台宏 | `P58_PreprocessorDemo.h/cpp` |

## P55 - Build.cs / Target.cs

**模块层级：**
```
Source/MyGame/
├── MyGame.Build.cs        ：模块构建配置
├── MyGame.Target.cs       ：可执行 target
├── MyGameEditor.Target.cs ：编辑器 target
├── MyGame.cpp             ：IMPLEMENT_PRIMARY_GAME_MODULE
└── MyGame.h
```

**Build.cs 关键字段：**
```csharp
PublicDependencyModuleNames.AddRange(new[] {
    "Core", "CoreUObject", "Engine", "InputCore"
});
PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });
PublicIncludePaths.AddRange(new[] { "..." });
PrivateIncludePaths.AddRange(new[] { "..." });
```

**API 导出宏：** `MYGAME_API`（自动生成 `Public/MyGame.h` 中）。`UCLASS(MYGAME_API)` 或类前缀 `class MYGAME_API UMyClass : public UObject` 控制是否跨模块可访问。

**新建子模块步骤：**
1. `Source/MyGameUtils/` 创建文件夹
2. 加 `MyGameUtils.Build.cs / .cpp / .h`
3. 在 `MyGame.uproject` 的 Modules 数组里加 entry
4. 主模块 Build.cs 添加 `"MyGameUtils"` 依赖

本项目演示所需的 4 个模块都已合并在 UETest 单模块 ；新建模块作为练习见上述步骤。

## P56 - Plugin

**目录结构：**
```
Plugins/MyPlugin/
├── MyPlugin.uplugin
├── Source/
│   └── MyPlugin/
│       ├── MyPlugin.Build.cs
│       ├── MyPlugin.cpp
│       └── ...
└── Resources/
    └── Icon128.png
```

**.uplugin 关键字段：**
```json
{
    "FileVersion": 3,
    "Version": 1,
    "FriendlyName": "My Plugin",
    "Modules": [
        {
            "Name": "MyPlugin",
            "Type": "Runtime",          // Runtime / Editor / Developer
            "LoadingPhase": "Default"   // Default / PreDefault / PostEngineInit ...
        }
    ]
}
```

**启用：** Project Settings → Plugins → 勾选 / 取消。

**GameFeature Plugin** ：UE5 推荐架构 ；按 Feature 切分 ；运行时可加载/卸载（Lyra 同款）。

## P57 - CVar / Command / Exec

**三种风格：**

```cpp
// 1. CVar ：全局变量 ，控制台输入修改
static float GDamageMult = 1.0f;
static FAutoConsoleVariableRef CVarDamageMult(
    TEXT("p2.DamageMultiplier"), GDamageMult,
    TEXT("全局伤害倍率"), ECVF_Cheat);

// 2. Command ：纯命令 ，绑 Lambda
static FAutoConsoleCommand CCmdKillAll(
    TEXT("p2.KillAll"), TEXT("杀死所有敌人"),
    FConsoleCommandDelegate::CreateLambda([]() { /* ... */ }));

// 3. Exec UFUNCTION ：在 Pawn / PC / GameMode 上识别
UFUNCTION(Exec) void DebugGodMode();
UFUNCTION(Exec) void DebugSetSpeed(float Speed);
```

**ECVF flags：**
- `ECVF_Default` ：可读可写
- `ECVF_Cheat` ：Shipping 中无效（防玩家滥用）
- `ECVF_RenderThreadSafe` ：可在渲染线程读
- `ECVF_ReadOnly` ：只读

**Exec 函数必须在 Pawn / PlayerController / Cheat Manager / GameMode / HUD 上才会被识别**。

## P58 - 预处理 / 平台宏

```cpp
#if WITH_EDITOR                  // 编辑器构建
    UE_LOG(LogTemp, Log, TEXT("仅编辑器"));
#endif

#if !UE_BUILD_SHIPPING           // Shipping 之外都执行
    void DebugDraw();
#endif

#if PLATFORM_WINDOWS / PLATFORM_MAC / PLATFORM_LINUX / PLATFORM_IOS / PLATFORM_ANDROID
```

**配置层级：** Debug / DebugGame / Development / Shipping / Test。`UE_BUILD_SHIPPING` 仅 Shipping 包为 1。

**TEXT() 宏：** UE 字符串字面量包装 ；产生 `wchar_t*` (Windows) 或 `char*` (其他平台)。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → G_工程化`
```
P57 Pawn bReplicates
P58 GetPlatformName 非空
```

**手动 PIE：**
- P57 ：控制台 `p2.DamageMultiplier 2.0` → CVar 改值 ；`p2.KillAll` → 触发 Command Lambda
- P57 ：把 Default Pawn Class 设为 `AP57_CVarCommandPawn` → 控制台输入 `DebugGodMode` 切换布尔
- P58 ：`UP58_PreprocessorHelper::IsShippingBuild()` 在 Development 构建返回 false

## 常见坑
- 新建模块忘了在 .uproject 注册 → 编译过但运行时找不到
- Plugin 模块 LoadingPhase 不对 → 启动顺序问题（Subsystem 还没创建就引用）
- CVar 名重复 → 后注册的覆盖前面的（无报错）
- Exec 函数放在 AActor 而不是 Pawn/PC → 控制台输入无效
- WITH_EDITOR 块里写"必须保留副作用"的代码 → Shipping 没有这段代码 ，行为不一致
