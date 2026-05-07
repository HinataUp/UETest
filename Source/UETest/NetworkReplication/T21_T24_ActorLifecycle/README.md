# T21-T24 Actor 生命周期与所有权

## 模块总览
理解 Actor 在网络上的"出生、归属、休眠、销毁"四个阶段，是定位"我的 Actor 怎么还没出现"等时序 bug 的基础。

| Task | 主题 | Actor |
|---|---|---|
| T21 | Spawn / Destroy 时序 | `AT21_LifecycleTimingActor` |
| T22 | `SetOwner` 与 Autonomous 判定 | `AT22_SetOwnerActor` |
| T23 | NetDormancy 四种状态 | `AT23_NetDormancyActor` |
| T24 | `bNetLoadOnClient` 与关卡 Actor | `AT24_NetLoadOnClientActor` |

完成后你能回答：
- Client BeginPlay 时初始 Replicated 属性是否可用？
- Autonomous Proxy 身份从何而来？
- NetDormancy 解决什么问题？
- 关卡 Actor 与 Spawn Actor 在 Client 上是怎么出现的？

## 使用方法

**T21：**
1. 关卡里放一个 `AT21_LifecycleTimingActor`
2. PIE → 三端 BeginPlay 日志中的 `InitialValue=999`（Client 端 BeginPlay 时已就绪）

**T22：**
1. 关卡里放 `AT22_SetOwnerActor`
2. PIE → 5 秒前 Player1 看到自己 Role=Simulated；5 秒后 Server 调 SetOwner，Player1 看到 Role 变为 Autonomous

**T23：**
1. 关卡里放 `AT23_NetDormancyActor`
2. PIE → 前 6 秒 Server 改值但 Client 看不到；第 6 秒 Server `FlushNetDormancy()` 一次，Client 收到最新

**T24：**
1. 关卡里放 `AT24_NetLoadOnClientActor`
2. 切换 `bDemoNetLoadFalse`，对比 Client 是否本地构造了该 Actor

## 实现要点

**T21 时序：**
- Server `BeginPlay` ≈ 立即；Client `BeginPlay` ≈ Server 复制到达 + 初始属性同步完成
- Server `Destroy()` → Client `EndPlay`（自动跟随）

**T22 SetOwner：**
```cpp
// 仅 Server 端有效
APlayerController* PC = GetWorld()->GetFirstPlayerController();
SomeActor->SetOwner(PC);
// 之后 PC 对应的 Client 上，Role 从 Simulated 变为 Autonomous
```

**T23 四种 NetDormancy：**
| 模式 | 行为 |
|---|---|
| `DORM_Awake` | 默认，正常复制 |
| `DORM_DormantAll` | 所有 Connection 都休眠 |
| `DORM_DormantPartial` | 按 `GetNetDormancy` 决定每个 Connection 是否休眠 |
| `DORM_Initial` | 初始休眠，需 `FlushNetDormancy` 才会做一次复制 |

```cpp
// 工程模式：地图大量 Pickup
NetDormancy = DORM_Initial;        // 初始休眠
// 玩家交互时
FlushNetDormancy();                // 唤醒做一次复制
SetNetDormancy(DORM_DormantAll);   // 处理完再睡
```

**T24 bNetLoadOnClient：**
```cpp
bNetLoadOnClient = true;   // 默认：Client 加载关卡时本地构造
bNetLoadOnClient = false;  // 跳过：Client 本地无此 Actor
```

## 原理简述

**初始属性同步保证：**
- UE 保证 Client BeginPlay 时初始 Replicated 字段已就绪
- 所以 BeginPlay 里读 Replicated 字段是安全的
- 但**运行时变化的字段**仍可能晚到，需要 OnRep 处理

**Autonomous 判定逻辑：**
```
ENetRole UActorChannel::GetRoleForLocalConnection()
{
    if (HasAuthority())                       return ROLE_Authority;
    if (Owner_Connection == LocalConnection)  return ROLE_AutonomousProxy;
    return ROLE_SimulatedProxy;
}
```
- "Owner_Connection" 沿 Actor.Owner → ... → PlayerController.NetConnection 链推导

**NetDormancy 的优化原理：**
- 默认每帧 Server 扫描每个 Replicated Actor 的所有字段，对比脏值
- Dormant Actor 整个跳过扫描；只在 Flush 时复制一次
- 适合"大部分时间稳定"的 Actor（地图物品）

**关卡 Actor vs Spawn Actor：**
| 来源 | Client 上怎么出现 | Server 销毁 |
|---|---|---|
| 关卡（Place in Level） | Client 加载关卡时本地构造（受 `bNetLoadOnClient` 控制）| 若 `bReplicates=true`，Client 同步销毁 |
| 运行时 Spawn | Server Spawn → 复制到 Client 自动构造 | Client 自动同步销毁 |

## 测试方法

**手动 PIE：**
- T21：日志比较 Server / Client 的 BeginPlay 时间差（PIE 通常很小，但能看出顺序）
- T22：关键验证 Role 从 Simulated → Autonomous 的切换瞬间
- T23：前 6 秒 Client 端 Val 始终为 0（休眠）→ Flush 后突然显示当前值
- T24：开关后启动 PIE，看 Client 窗口里这个 Actor 是否存在

**自动化测试：** `UETest.Network.生命周期与组件`

```
覆盖：
  T21 bReplicates 默认 true
  T23 NetDormancy 默认应为 DORM_Initial
  T24 默认 bNetLoadOnClient 应为 true
```

注：**实际时序行为**单测覆盖不到，必须 PIE 观察。

## 常见坑
- 在 Client 端 BeginPlay 里假设 Replicated **指针**字段已就绪 → 可能 null（指针解析晚于值同步）
- 想运行时 SetOwner 让 Server RPC 工作 → 必须 Server 端调，Client 调无效
- Dormant Actor 改了值不 Flush → Client 永远看不到
- 关卡 Actor `bReplicates=false` 但 Server `Destroy()` → Client 上仍存在（销毁不同步）
