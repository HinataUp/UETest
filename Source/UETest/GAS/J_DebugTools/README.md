# J 章节 - 工程化与调试 (J47-J49)

## 模块总览
GAS 调试速查表 + Dump 工具 + 常见 Bug 自查清单。日常开发提效核心。

| Task | 文件 | 主题 |
| --- | --- | --- |
| J47 | 见本 README "一、控制台命令族" | GAS 内置调试命令 |
| J48 | `J48_ASCDumpHelper.h/cpp` | ASC 状态 Dump 到文件（三端 Diff）|
| J49 | 见本 README "三、常见 Bug 自查清单" | 排查流程沉淀 |

## 一、控制台命令族（J47）

### 屏幕调试

| 命令 | 用途 |
| --- | --- |
| `showdebug abilitysystem` | 屏幕显示当前 ASC 状态（Tag/Attr/ActiveGE）|
| `AbilitySystem.Debug.NextTarget` | 切换查看的目标 Actor |
| `AbilitySystem.Debug.PrevTarget` | 反向切换 |
| `AbilitySystem.Debug.NextCategory` | 切换显示类别（Attributes / Tags / GameplayEffects 等）|

### 日志开关

| 命令 | 用途 |
| --- | --- |
| `log LogAbilitySystem Verbose` | GAS 框架详细日志 |
| `log LogGameplayEffects Verbose` | GE 应用 / 移除 / Modifier 聚合 |
| `log LogGameplayCue Verbose` | Cue 触发 / Notify 注册 |
| `log LogGASTest Verbose` | 本项目 `GAS_VLOG` 详细日志（默认关闭）|

### 弱网模拟（与网络同步模块共用）
```
net.PktLag 200
net.PktLagVariance 50
net.PktLoss 20
```

## 二、ASC Dump 工具（J48）

```cpp
// 在三端任意位置调用
UASCDumpHelper::DumpASCState(MyASC, TEXT("Server"));   // Server 端调
UASCDumpHelper::DumpASCState(MyASC, TEXT("Client1"));  // Client1 端调
UASCDumpHelper::DumpASCState(MyASC, TEXT("Client2"));  // Client2 端调

// 输出文件位于 ProjectLogDir/ASC_Dump_<Label>.txt
// 用 diff 工具比较三个文件 → 差异即为同步 Bug 的线索
```

文件包含：
- 所有 Spawned AttributeSet 的 Base / Current 值
- 所有 Owned Tags
- 所有 ActiveGameplayEffects 数量

## 三、常见 Bug 自查清单（J49）

### 症状："属性没同步到 Client"
- [ ] `DOREPLIFETIME_CONDITION_NOTIFY` 写了吗？COND 是否正确？
- [ ] `OnRep_Xxx` 里调了 `GAMEPLAYATTRIBUTE_REPNOTIFY` 吗？
- [ ] ASC 本身 `SetIsReplicated(true)` 了吗？
- [ ] ReplicationMode 是不是 Minimal 过滤了你想看的字段？
- [ ] 字段是不是 `COND_OwnerOnly` 但你在非 Owner 端读？

### 症状："GA 激活失败"
- [ ] `GiveAbility` 在 Server 端调了吗？
- [ ] Tag 要求满足吗？（`showdebug abilitysystem` 看当前 Tag）
- [ ] Cost / Cooldown 检查通过吗？
- [ ] NetExecutionPolicy 和调用端匹配吗？（LocalOnly 在 Server 触发会失败）
- [ ] `CanActivateAbility` 没被你 override 错？

### 症状："预测行为异常"
- [ ] NetExecutionPolicy 是 LocalPredicted 吗？
- [ ] 改动是通过 GE / Cue 走 GAS 管线吗？（直接 SetNumericAttributeBase 不走预测）
- [ ] Server 端 CanActivate 返回了 false 吗？（拒绝 → 回滚）
- [ ] PredictionKey 看起来 `IsValidKey()` 吗？

### 症状："Cue 没触发"
- [ ] Tag 前缀是 `GameplayCue.` 吗？
- [ ] CueNotify 资产在 CueManager 扫描路径下吗？
- [ ] 用 Execute 还是 Add？（持续型 Burning 必须用 Add）
- [ ] AssetTags 配置正确吗？

## 四、自动化测试

J 章节是工具/方法，不需要专门 Spec 测试。日常用法：

1. 跑前面所有 Spec 测试（`UETest.GAS.*`）
2. 实际战斗中报 Bug → 三端 Dump → diff 找差异
3. 顽固问题 → `log LogGameplayEffects Verbose` + `log LogAbilitySystem Verbose` 全开

## 五、推荐调试套餐（PIE）

```
showdebug abilitysystem
AbilitySystem.Debug.NextCategory
log LogGameplayEffects Verbose
net.PktLag 100
net.PktLagVariance 20
```

跑战斗就能看到：屏幕显示 ASC 状态 + Output Log 显示 GE 应用细节 + 100ms 延迟体感。

## 常见坑
- 屏幕调试只显示当前选中的目标 ：用 `NextTarget` 切到你想看的角色
- `log LogGameplayEffects Verbose` 输出量大 ：调试结束记得 `log LogGameplayEffects Log` 关掉
- Dump 函数在打包后 ：检查 `bUsesDeveloperTools` 是否启用，否则 SaveStringToFile 可能写不到位置
- 三端 Dump 时间戳不同步 → 用同一时刻命令面板触发（控制台同时回车）
