# I 章节 - GAS 网络 (I43-I46)

## 模块总览
GAS 在多人项目下的网络细节。前面章节默认走 LocalPredicted/Mixed 模式，本章节剖析这些选择背后的机制。

| Task | 文件 | 主题 |
| --- | --- | --- |
| I43 | （见 `00_Common/GASTestPlayerState/AICharacter`）| Mixed vs Minimal 复制模式 |
| I44 | （见 `A_Basics/A02_CombatSet`）| OwnerOnly 私有属性 |
| I45 | `I45_GA_PredictionKey.h/cpp` | PredictionKey 观察 |
| I46 | `I46_GA_PredictionRollback.h/cpp` | LocalPredicted 完整闭环（含拒绝回滚）|

## 使用方法

**I43 Replication Mode：**
- Player ASC：在 PlayerState 上，Mixed 模式（已在 `AGASTestPlayerState` 配置）
- AI ASC：在 Character 上，Minimal 模式（已在 `AGASTestAICharacter` 配置）

**I44 OwnerOnly 属性：**
- `Mana` / `MaxMana` 用 `COND_OwnerOnly` 复制
- 别的玩家看不到自己的 Mana ：竞技场景常用

**I45 观察 PredictionKey：**
- 触发 `UGA_PredictionKey`（LocalPredicted）
- 对照 Server 与 Client 日志 ：同一个 Key 值

**I46 预测拒绝：**
- 触发 `UGA_PredictionRollback` 多次
- Server 50% 概率拒绝 → Client 已做的预测改动被 GAS 自动撤销

## 实现要点

**Replication Mode 选择规则：**
| 模式 | ASC 应放在 | 复制内容 |
| --- | --- | --- |
| Full | 不推荐多人 | 全部 GE 详情广播给所有人 |
| **Mixed** | **PlayerState（不能是 Pawn）** | Owner 收完整，Simulated 收必要 Tag/属性 |
| **Minimal** | Character / Pawn | 仅必要 Tag/属性 |

**为什么 Mixed 必须在 PlayerState：**
- Pawn 是 Simulated（其他玩家看你的角色）+ Autonomous（你自己控制）
- Mixed 模式假设 ASC.OwnerActor 是"持久存在 + 跨 Connection 不变"的对象 → 只有 PlayerState 满足
- 把 Mixed ASC 放 Pawn 上 ：Simulated 端 Tag 复制异常

**OwnerOnly 私有属性：**
```cpp
DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet, Mana, COND_OwnerOnly, REPNOTIFY_Always);
```
- Server 只把这个字段发给 Owner Connection
- 其他 Client 拿到的 Mana 是默认值（没收到复制）

**PredictionKey 流程（I45）：**
1. Client 端 LocalPredicted GA 激活 → GAS 生成本地 Key（递增）
2. 发 RPC 给 Server，带上 Key
3. Server 端 ActivateAbility 时 `ActivationInfo.GetActivationPredictionKey()` 拿到同一 Key
4. Client 期间所有 GE / Cue 改动都关联到这个 Key
5. Server 处理完发回确认 ：一致 → Client 端 Key 关联清空；不一致 → 自动回滚

## 原理简述

**LocalPredicted 闭环：**
```
[Client] TryActivate → 生成 PredictionKey K
       → 本地 ActivateAbility（Cost扣、CD计、GE应用）all marked with K
       → 发 RPC ServerTryActivate(K)

[Server] 收到 RPC
       → CanActivateAbility ？
            true  → ActivateAbility（带 K）→ 复制结果回 Client
            false → 发 RPC 让 Client 回滚 K 关联的所有改动
```

**为什么 Server 拒绝时 Client 能"自动"回滚：**
- Client 应用 GE 时，ActiveGE 内部记 `PredictionKey=K`
- Server 端拒绝 → 通过 RPC 告诉 Client：撤销 K 关联的全部 ActiveGE
- Client GAS 框架自动遍历 ActiveEffects 撤销

**预测的边界：**
- Attribute 直接 `SetNumericAttributeBase` ：不走预测管线，没法回滚
- Apply Instant GE 改 BaseValue ：走预测，可以回滚
- Cue 的 OnExecute ：可以预测；Cue 的 OnRemove 不行
- 实战 ：所有"想被预测"的改动必须通过 GE / Cue 走 GAS 标准管线

## 测试方法

**自动化测试：** `UETest.GAS.G_Tag与H_集成与I_网络`
```
I46 PredictionRollback GA 配置正确（NetExecutionPolicy=LocalPredicted）
```

**手动 PIE（这章主要靠 PIE）：**

**I43 Replication Mode：**
1. Listen Server + 2 Clients
2. Player ASC 应用 GE_AtkBuff5s
3. `showdebug abilitysystem` 在 Client2 上 ：能看到 Player 的 Buff Tag/属性 ；AI 的 Buff 信息更精简

**I44 OwnerOnly：**
1. Player1 当前 Mana=80
2. Client1 本地查 Player1 的 Mana → 80 ✓
3. Client2 查 Player1 的 Mana → 0（未收到复制）
4. 但 Client2 查 Player1 的 Health → 正确值（COND_None）

**I45 PredictionKey：**
1. 触发 `UGA_PredictionKey`
2. Client 日志 ：`Current=42 IsLocalClientKey=1 IsValid=1`
3. Server 日志 ：`Current=42 IsLocalClientKey=0 IsValid=1`
4. 同一个 Key 值

**I46 预测回滚：**
1. `net.PktLag 100`
2. 触发 `UGA_PredictionRollback` 多次
3. 拒绝的次数（约 50%）：Client 看到瞬间预测应用 → ~100ms 后被回滚
4. Client 日志会显示 GE 撤销

## 常见坑
- Player ASC 放 Pawn ：Mixed 模式下 Simulated Tag 异常
- AI ASC 用 Mixed 模式 ：浪费带宽（Minimal 才合适）
- 期望直接 `SetNumericAttributeBase` 也能预测 ：不行，必须走 GE
- LocalPredicted GA 但 Server 不实现 `_Validate` ：作弊空间
- `IsLocalClientKey` 在 Server 端永远 false ：不要用这个判 Local/Server，用 `HasAuthority`
