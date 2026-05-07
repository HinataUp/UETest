# T1-T5 属性复制基础

## 模块总览
属性复制是 UE 网络同步的两根支柱之一（另一根是 RPC）。本目录把"按值同步"的所有常见类型走一遍。

| Task | 主题 | Actor |
|---|---|---|
| T1 | `bReplicates` 与 `Replicated` 标记 | `AT1_BReplicatesActor` |
| T2 | 基础类型（int/float/bool/FString/FName）| `AT2_BasicTypesActor` |
| T3 | USTRUCT 整体复制 | `AT3_StructActor` |
| T4 | TArray 容器（含结构体数组）| `AT4_ArrayActor` |
| T5 | 枚举与 FName 低成本类型 | `AT5_EnumNameActor` |

完成后你能回答：
- Actor 要复制需要哪些开关？
- 字段 / 结构体 / 容器在网络上的体积差异有多大？
- 为什么 USTRUCT 是"原子单位"复制？
- UENUM 为什么必须 `: uint8`？

## 使用方法
1. 把每个 Actor 拖一份到关卡（不需要 SetOwner）
2. 三窗口 PIE 启动
3. Server 端用 Timer 自动每 1-2 秒改值；三端同时打日志
4. 对比 Server 端写入值 与 Client 端收到值，确认同步成功

## 实现要点

**两个开关都要开：**
```cpp
// 1) 构造期声明 Actor 参与复制
bReplicates = true;

// 2) 字段加 UPROPERTY(Replicated)
UPROPERTY(Replicated) int32 Value;

// 3) 在 GetLifetimeReplicatedProps 注册
DOREPLIFETIME(MyClass, Value);
```

**三种 DOREPLIFETIME 写法对比：**
| 宏 | 用法 | 何时选 |
|---|---|---|
| `DOREPLIFETIME` | 默认条件复制 | 大部分场景 |
| `DOREPLIFETIME_CONDITION` | 带 COND_xxx | 见 T14 |
| `DOREPLIFETIME_WITH_PARAMS_FAST` | UE5 Push Model | 高频字段优化 |

## 原理简述

**属性复制是状态同步**，不是事件同步：
- Server 改 `Value = 5`，Client 最终能看到 `5`
- 但 Server 同帧改 `0→1→2→3→4→5`，Client 只收到一次"=5"，中间过程丢失（详见 T9）

**结构体（USTRUCT）的"原子性"：**
- 内部任何一个字段变 → 整个结构体重传
- 适合：相关性强、变化频率接近的字段聚成组
- 不适合：独立变化的字段，单独 UPROPERTY(Replicated) 更省

**TArray 复制：**
- UE 内部按元素 Diff
- 增/改/删都正确同步，但**索引顺序在两端不保证完全一致**
- 大数组高频改动是带宽热点，进阶用 `FastArraySerializer`（GAS 走的就是这个）

**枚举 / FName 为什么便宜：**
- `UENUM : uint8` 实际占 1 byte，网络上最多 1 byte
- `FName` 底层是全局表索引，传索引不传字符串

## 测试方法

**手动 PIE：**
- Server 端日志显示 `Tick i=N`，Client 端应跟随收到相同 N
- T1：先注释掉 `Replicated` 标记跑一次（Client 永远 0），再加上跑一次（Client 同步）
- T4：观察 Add / Modify / Remove / Empty 四阶段在 Client 上都正确反映

**自动化测试：** `UETest.Network.属性复制`

```
Session Frontend → Automation → 搜 UETest.Network.属性复制
覆盖：
  T1 bReplicates 与 Replicated 标记
    - Actor 默认应启用 bReplicates
    - T1_TestValue 字段必须被注册到复制系统
    - 没标 Replicated 的字段不应被注册
  T2 五个字段必须全部注册到复制系统
  T3 T3_CombatState 字段必须注册 / 内部字段反射可读
  T4 两个 TArray 字段必须注册
  T5 ET5_CombatPhase 必须是 uint8 底层
```

## 常见坑
- 忘了写 `GetLifetimeReplicatedProps` → 启动 warning：`Property X needs to be registered for replication`
- `Replicated` 标记和 `DOREPLIFETIME` 缺一不可
- 频繁同步长 FString 是性能反模式，改用 FName 或 ID
