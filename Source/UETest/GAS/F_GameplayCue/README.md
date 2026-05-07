# F 章节 - GameplayCue (F33-F34)

## 模块总览
表现层广播。属性走 GE（Reliable），视觉/音效走 Cue（Unreliable Multicast，丢一帧无所谓）。

| Task | 文件 | 主题 |
| --- | --- | --- |
| F33 Burst | `F33_HitFireCueNotify.h/cpp` | Static 型 Cue ，OnExecute 一次性触发 |
| F33 Actor | `F33_BurningCueNotify.h/cpp` | Actor 型 Cue ，OnActive/WhileActive/OnRemove 持续 |
| F34 | 见本 README "原理简述" | Cue 同步机制与丢失容忍（无独立代码）|

## 使用方法

**触发 Burst Cue（一次性）：**
```cpp
// 直接调用
ASC->ExecuteGameplayCue(MyGameplayTags::TAG_GameplayCue_Hit_Fire, CueParams);

// 或在 GE 里配 GameplayCues 数组（Burst Cue 配 OnExecute）
```

**触发 Actor Cue（持续性）：**
```cpp
// Add ：开始持续表现
ASC->AddGameplayCue(MyGameplayTags::TAG_GameplayCue_Buff_Burning, CueParams);

// Remove ：结束
ASC->RemoveGameplayCue(MyGameplayTags::TAG_GameplayCue_Buff_Burning);

// 或在 GE 里配 GameplayCues（Actor Cue 用 OnActive/WhileActive/OnRemove）
```

## 实现要点

**Cue Tag 命名约定：**
- 必须以 `GameplayCue.` 开头 ，CueManager 才会扫描注册
- 例：`GameplayCue.Hit.Fire` / `GameplayCue.Buff.Burning`

**Burst Cue 子类（UGameplayCueNotify_Static）：**
```cpp
virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Params) const
{
    // 一次性表现 ：SpawnEmitterAtLocation + PlaySound
    return true;
}
```

**Actor Cue 子类（AGameplayCueNotify_Actor）：**
```cpp
virtual bool OnActive_Implementation(...);    // 进入态
virtual bool WhileActive_Implementation(...); // 中途加入的观察者也能看到
virtual bool OnRemove_Implementation(...);    // 退出态
```

**注册路径：**
- 默认扫描 `/Game/...` 下的 BP/CueNotify
- 项目可在 `DefaultGame.ini` 配 `[/Script/GameplayAbilities.AbilitySystemGlobals] GameplayCueNotifyPaths=...`

## 原理简述

**Cue 同步机制（F34）：**
- Cue 底层是优化过的 **Unreliable Multicast** —— 容忍丢失，省带宽
- 关键状态走 GE（Reliable）：HP 变化必到达
- 表现走 Cue（Unreliable）：粒子丢一次无所谓

**为什么 Cue 可以丢：**
- Burst Cue 是"事件" ：丢了下次还能再来，没有累积错误
- Actor Cue 有 `WhileActive` 兜底：中途加入的观察者也能"看到当前在燃烧"

**节流：** 高频 Cue（每帧伤害血花）必须在 ASC 端节流，否则撑爆带宽。

**手动 vs GE 触发：**
| 触发方式 | 适用 |
| --- | --- |
| 代码 `ASC->ExecuteGameplayCue` | 即兴的命中表现 |
| 代码 `ASC->AddGameplayCue` | 持续状态进入 |
| GE 内置 GameplayCues | 与某 GE 强绑定的表现（Buff 自带光环）|

## 测试方法

**手动 PIE（这章主要靠 PIE）：**

**F33 Burst：**
1. 控制台 `ASC->ExecuteGameplayCue(GameplayCue.Hit.Fire)` 或在某 GE 内配 GameplayCues
2. 观察 OnExecute 日志在三端都触发

**F33 Actor：**
1. `ASC->AddGameplayCue(GameplayCue.Buff.Burning)` → 三端 OnActive
2. 中途连入的 Client → 触发 WhileActive
3. `RemoveGameplayCue` → 三端 OnRemove

**F34 丢失容忍：**
1. PIE 控制台 `net.PktLoss 30`
2. 连续 ExecuteGameplayCue 10 次 ：远端 Client 可能只看到 7-8 次
3. 同时 Apply 一个改 HP 的 GE：HP 一定到达（Reliable）

**自动化测试：** 此章节涉及 Cue Manager 扫描 + 网络复制，单测覆盖困难，建议手动验证。

## 常见坑
- Tag 命名不带 `GameplayCue.` 前缀 → CueManager 不扫描，触发无反应
- CueNotify 资产放错路径 → 需检查 `GameplayCueNotifyPaths` 配置
- 持续型用 Execute 不是 Add → OnActive/OnRemove 不被调用，只触发一次 OnExecute
- Reliable Cue ：极少需要，只有"必须到达的关键提示"（如关键 BOSS 阶段）才用
- 高频 Cue 不节流 → 带宽爆炸
