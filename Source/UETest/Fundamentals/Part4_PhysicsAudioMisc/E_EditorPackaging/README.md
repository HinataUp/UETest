# E 章节 - Editor Tools + 打包 (P97-P98)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P97 | Editor Utility Widget + 批量操作 | `P97_BatchActorHelper.h/cpp` |
| P98 | Build Configuration / Cooking | `P98_BuildInfoHelper.h/cpp` |

## P97 - Editor Utility Widget

**思路：** Editor Utility Widget（EUW）是编辑器内运行的 BP UI ；可绑按钮调用 BP / C++ 函数 ；做工具 ：批量重命名 / 批量改属性 / 资产校验。

**前置：** Plugins → 启用 "Editor Scripting Utilities" + "Blutility"

**步骤：**
1. Content Browser 右键 → Editor Utilities → Editor Utility Widget
2. 在 Designer 中加按钮
3. 按钮 OnClicked ：调本项目 `UP97_BatchActorHelper::AddTagToActors(SelectedActors, FName("Reviewed"))`
4. 编辑器右键 EUW 资产 → Run Editor Utility Widget → 显示窗口

**获取选中 Actor（编辑器）：** 在 EUW BP 中 ：`Get Selected Level Actors` 节点。

**ActorActionUtility（右键菜单扩展）：**
```cpp
UCLASS()
class UMyActorActions : public UActorActionUtility {
    GENERATED_BODY()
    UFUNCTION(CallInEditor, Category = "MyTools")
    void DoSomething();
};
```
- 编辑器选中 Actor → 右键菜单 Scripted Actions → DoSomething

## P98 - Packaging / Cooking

**Build Configuration：**
| 配置 | 用途 |
| --- | --- |
| Debug | 完整调试信息 ；最慢 |
| DebugGame | Engine 用 Development，Game 用 Debug |
| Development | 开发期默认 ；含调试输出 |
| Test | 接近 Shipping 但保留少量 Cheat |
| Shipping | 发布版 ；最优化 ；调试代码编译掉 |

**Cook 流程：**
1. Project → Package Project → 选目标平台 + 配置
2. UAT 调用 UnrealEditor cook → 序列化所有引用资产为 .uasset
3. 打包 .pak / .ucas / .utoc
4. 输出可分发包

**关键宏：**
```cpp
#if WITH_EDITOR        // 编辑器构建
#if !UE_BUILD_SHIPPING // Shipping 之外
#if UE_BUILD_TEST      // Test 配置
```

**验证打包正确性：**
- `FMessageLog` 输出 Cook 阶段警告 / 错误
- 检查 Output Log 的 LogPackaging
- 跑包内 Smoke Test
- 检查无 Missing Asset 引用

**本 Helper 提供 BP 调用接口判断当前构建配置：**
```cpp
EP98_BuildConfig C = UP98_BuildInfoHelper::GetBuildConfiguration();
if (UP98_BuildInfoHelper::IsShippingBuild()) { /* 隐藏调试 UI */ }
```

## 测试方法

**自动化测试：** `UETest.Fundamentals.P4_物理音频特效 → E_编辑器与打包`
```
P97 AddTagToActors 行为正确
P98 GetBuildConfiguration 不为 Unknown
P98 IsEditorBuild + HasDebugOutput 配对正确
```

**手动验证：**
- P97 ：启用 EUW 插件 → 创建 EUW BP → 按钮调 `AddTagToActors`
- P98 ：用不同 Build Configuration 打包 → 包内调 `IsShippingBuild` 看不同结果

## 常见坑
- EUW 没启用 EditorScripting 插件 → 看不到 Editor Utilities 菜单
- ActorActionUtility 子类没 `CallInEditor` → 右键菜单不显示
- Shipping 包内 `check()` 被编译掉 → 测试期靠 `check` 防御的逻辑在 Shipping 中变成无防御
- ensure 在 Shipping 中编译掉 ；想保留断言但不崩溃 → 用 `verify` 或 `check`
- Cook 过程中 Editor Only 资产被错误打包 → 在资产 Details 标记 "Editor Only"
- Missing Asset 引用 → Cook 警告但不致命 ；运行时变 nullptr ；养成检查习惯
