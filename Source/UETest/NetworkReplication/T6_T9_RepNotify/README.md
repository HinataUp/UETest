# T6-T9 RepNotify（OnRep 回调）

## 模块总览
属性复制只让数据"到"了 Client，但 Client 怎么知道数据变了？**ReplicatedUsing** 让你声明一个 OnRep 回调，UE 在属性到达时自动调用——这是反应式编程的核心，也是 Simulated Proxy 做表现层响应的标准手段。

| Task | 主题 | Actor |
|---|---|---|
| T6 | 无参 OnRep | `AT6_OnRepBasicActor` |
| T7 | 带 OldValue 参数 | `AT7_OnRepWithOldValueActor` |
| T8 | `REPNOTIFY_Always` + Authority 手动调 OnRep | `AT8_RepNotifyAlwaysActor` |
| T9 | 同帧多次修改的合并行为 | `AT9_BatchedChangesActor` |

完成后你能回答：
- OnRep 在 Authority / Autonomous / Simulated 三端各自触发吗？
- OldValue 来自哪里？
- 想让 Server 也走 OnRep 怎么写？
- 同帧改 5 次属性，Client 收到几次 OnRep？

## 使用方法
1. 把对应 Actor 拖入关卡
2. 三窗口 PIE
3. Server 自动用 Timer 改值；三端同时打日志，对比 OnRep 触发次数

## 实现要点

**几种 OnRep 写法：**
```cpp
// 写法1：无参
UPROPERTY(ReplicatedUsing = OnRep_Health) int32 Health;
UFUNCTION() void OnRep_Health();

// 写法2：带 OldValue（值类型）
UFUNCTION() void OnRep_Health(int32 OldHealth);

// 写法3：带 OldValue（结构体引用，避免拷贝）
UFUNCTION() void OnRep_State(const FCombatState& OldState);
```

**关键约束：**
- OnRep 必须 `UFUNCTION()` 修饰，否则反射找不到，UE 不会调用
- 函数命名约定 `OnRep_<FieldName>`，强约定，偏离让协作者困惑
- 签名必须严格匹配，否则 OldValue 不会被正确传递

**让 Server 也走 OnRep（标准模式）：**
```cpp
// 1) DOREPLIFETIME_CONDITION_NOTIFY 改用 REPNOTIFY_Always
DOREPLIFETIME_CONDITION_NOTIFY(MyClass, Stacks, COND_None, REPNOTIFY_Always);

// 2) Server 端业务函数：手动调 OnRep
void MyClass::ServerRefresh(int32 NewStacks)
{
    const int32 Old = Stacks;
    Stacks = NewStacks;
    if (HasAuthority()) { OnRep_Stacks(Old); }
}
```

## 原理简述

**OnRep 触发规则：**
| 端 | 是否触发 |
|---|---|
| Authority | 默认不触发（Server 自己改的，知道自己干了什么）|
| Autonomous Proxy | 触发 |
| Simulated Proxy | 触发 |

**OldValue 来源：**
- **Client 本地上次复制到的值**，不是 Server 的历史
- 丢包重传场景下，OldValue 仍是 Client 看到的最近值

**REPNOTIFY_OnChanged vs REPNOTIFY_Always：**
| 模式 | 行为 |
|---|---|
| `OnChanged`（默认）| 值真的变了才触发 OnRep |
| `Always` | 只要发生复制就触发，即使值未变 |

**同帧合并：**
属性复制是**状态同步不是事件同步**——Server 同帧改 5 次，UE 在网络包里只发"最终值"，Client 只触发 1 次 OnRep。需要事件序列请用 Multicast RPC 或带历史数组（见 T9 的 `T9_HitHistory`）。

## 测试方法

**手动 PIE：**
- T6：Server 端不打 OnRep 日志，Client 端每秒打一次
- T7：先扣血几次再加血，OnRep 中 OldValue 反映"上次 Client 看到的"
- T8：用 `REPNOTIFY_Always`，Server 设回相同值时 Client 仍触发 OnRep
- T9：Server `Burst` 5 次后看 Client 日志——只 1 次 OnRep，但 `T9_HitHistory.Num() == 5`

**自动化测试：** `UETest.Network.RepNotify`

```
覆盖：
  T6 OnRep_T6_RepInt 函数被反射注册
     T6_RepInt 字段已注册到复制系统
  T7 OnRep_T7_Health 函数被反射注册且参数数=1
  T8 Server 手动调 OnRep 不会崩溃，能正常更新内部状态
  T9 HitCount 与 HitHistory 数组字段都已注册
```

注：**OnRep 是否真在远端触发**这件事单测无法验证，必须靠 PIE 观察日志。

## 常见坑
- OnRep 函数没加 `UFUNCTION()` → 静默不触发，调试半天
- 函数签名写错（如 `OnRep_X(int32 OldVal, bool bAlsoOld)`）→ UE 视作无参 OnRep
- 想让 Server 也走 OnRep → 单独加 `REPNOTIFY_Always` 不够，必须 Server 手动 `OnRep_X(Old)`
- 把"事件计数"做成 `Replicated int HitCount` → 跨端会丢中间帧
