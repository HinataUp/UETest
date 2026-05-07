# T14-T15 条件复制

## 模块总览
默认情况下，一个 Replicated 字段会同步给所有"看得到这个 Actor"的 Client。但很多时候你想更精细——只发给 Owner？只发给远端他人？只在战斗中同步？条件复制就是干这个的。

| Task | 主题 | Actor |
|---|---|---|
| T14 | 五种 `DOREPLIFETIME_CONDITION`（静态条件）| `AT14_LifetimeConditionActor` |
| T15 | `PreReplication` 动态条件 | `AT15_PreReplicationActor` |

完成后你能回答：
- `COND_OwnerOnly` 和 `COND_SkipOwner` 分别适合什么场景？
- 如何实现"运行时动态决定一个字段要不要复制"？

## 使用方法

**T14：**
1. 由于 OwnerOnly / SkipOwner / AutonomousOnly 需要 Owner 才能观察到，建议把这个 task **放在玩家 Pawn / PlayerState 上**跑（或先在场景 Actor 上 SetOwner，参 T22）
2. 三窗口 PIE → 三端日志对比 5 个字段的可见性

**T15：**
1. 关卡放一个 `AT15_PreReplicationActor`
2. PIE → 每秒 Server 改值；每 5 秒翻转开关
3. 观察 Client：开关 OFF 期间，Client 看到的值停滞；开关 ON 后追上

## 实现要点

**T14 五种条件速查：**
```cpp
DOREPLIFETIME_CONDITION(C, F, COND_OwnerOnly);       // 只给 Owner
DOREPLIFETIME_CONDITION(C, F, COND_SkipOwner);       // 除 Owner 外都同步
DOREPLIFETIME_CONDITION(C, F, COND_SimulatedOnly);   // 只给"看别人"的端
DOREPLIFETIME_CONDITION(C, F, COND_AutonomousOnly);  // 只给"控制本 Pawn"的玩家
DOREPLIFETIME_CONDITION(C, F, COND_InitialOnly);     // 只首次到达时同步
```

**典型用途：**
| COND | 战斗中的典型场景 |
|---|---|
| OwnerOnly | 背包内容、技能冷却（私有信息）|
| SkipOwner | Owner 已本地处理过的状态，不重复发 |
| SimulatedOnly | 只远端表现需要的数据（旁观者视角的姿态）|
| AutonomousOnly | 输入修正、Server 给自机的纠正 |
| InitialOnly | 队伍 ID、初始装备（一辈子不变）|

**T15 动态条件：**
```cpp
void MyActor::PreReplication(IRepChangedPropertyTracker& Tracker)
{
    Super::PreReplication(Tracker);
    DOREPLIFETIME_ACTIVE_OVERRIDE(MyActor, MyField, bShouldReplicate);
}
```
- 每次复制本 Actor 之前调用一次 `PreReplication`
- `_ACTIVE_OVERRIDE` 的第三参为 bool：`true` 允许同步，`false` 屏蔽
- UE5 还有 `_FAST` 变体，配合 Push Model 优化（高频字段）

## 原理简述

**Connection 视角的过滤：**
- Server 为每个 Connection 维护一份"该 Connection 能看到哪些字段"的位图
- COND_xxx 决定字段是否进入某 Connection 的复制包

**为什么 OwnerOnly 需要 SetOwner：**
- Owner 链：Actor.Owner → ... → PlayerController → Connection
- 没 Owner 的 Actor 在所有 Connection 看来都不是"我的"，所以 OwnerOnly 等于"不发任何人"

**动态屏蔽 vs 动态值：**
- 动态屏蔽（T15）：Client 收不到新值 → 仍持有上次的值，不会清零
- 想"清空数据"必须显式赋值 → 触发一次复制

## 测试方法

**手动 PIE：**
- T14 必须借助 Pawn 才能观察 OwnerOnly 等条件——简便做法是把字段加到默认 Character 上跑
- T15：观察 `Replicating=0/1` 切换时 Client 数值是否停滞 / 追赶

**自动化测试：** `UETest.Network.生命周期与组件 → T14`

```
覆盖：
  T14 五种 DOREPLIFETIME_CONDITION
    - 五个条件字段全部注册
    - OwnerOnly 字段的 RepCondition 必须是 COND_OwnerOnly
  T15 PreReplication 动态条件
    - T15_ConditionalField 已注册
```

注：**条件实际生效**这件事单测无法验证（需要 Connection 上下文），用反射检查 `FLifetimeProperty.Condition` 字段是退而求其次。

## 常见坑
- 在场景 Actor 上跑 OwnerOnly 但忘了 SetOwner → 五个 COND 看起来全部"屏蔽"
- `PreReplication` 每帧 Server 调一次，放重逻辑会拖性能
- 误以为关掉 ACTIVE_OVERRIDE 就清空 Client 的值 → 不会，Client 保留旧值
- COND_InitialOnly 的字段后续修改不会被同步，要"初始化后即不变"才用
