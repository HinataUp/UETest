# T0 - 网络同步测试基础工程

## 模块总览
所有后续 task 的载体。本目录提供：
- `ANetTestActor`：最简可复制 Actor，带白色立方体 Mesh，便于在场景中识别
- `../Common/NetTestLogMacros.h`：统一 `NET_LOG` 宏，自动带 `[NetMode|Role|ActorName]` 前缀

完成后你能回答：
- 一个没设 Owner 的 Actor 在 Client 上是什么 Role？
- `bReplicates` 决定什么？字段的 `Replicated` 决定什么？

## 使用方法
1. 在 Content Browser 右键 → Place Actors → 搜索 `NetTestActor`，拖入关卡
2. Editor → Play → Number of Players = 3 → Net Mode = **Play As Listen Server**
3. Editor Preferences → Play → Use Single Process **关闭**（多进程才有独立 Output Log）
4. 三个独立窗口的 Output Log 观察 `BeginPlay` 日志

## 实现要点
- `bReplicates = true` 在构造函数里设最稳定；运行时改用 `SetReplicates(true)`
- `CreateDefaultSubobject<UStaticMeshComponent>` 创建的组件两端有同一 NetGUID
- 命名加 `NT_` 前缀（NetTest）避免与 UE 内置命名冲突
- 默认 Mesh 用 `/Engine/BasicShapes/Cube.Cube`，避免依赖外部资产

## 原理简述
**Net Role 三种身份**：

| Role | 出现在 | 含义 |
|---|---|---|
| `Authority` | Server | Actor 的"真身"，权威逻辑发生地 |
| `AutonomousProxy` | 拥有该 Actor 的 Client | 玩家自己的 Pawn |
| `SimulatedProxy` | 其他 Client | 看到的别人的 Pawn |

判定 `Autonomous` 的规则：`Actor.Owner` 链追溯到某 `PlayerController`，且该 PC 对应的 Connection 与本端相同 → 本端是 Autonomous。

`ANetTestActor` 没 Owner，所以在所有 Client 上都是 Simulated。

## 测试方法

**手动 PIE：**
- Server 窗口期待：`[Listen|ROLE_Authority|NetTestActor_X] BeginPlay`
- Client 窗口期待：`[Client|ROLE_SimulatedProxy|NetTestActor_X] BeginPlay`

**自动化测试：** 本基础设施由后续 task 间接验证（NewObject 出 Actor 时检查默认配置）。

## 常见坑
- 忘了关 Use Single Process → 三窗口日志全混在一个 Output Log 里
- 把 `bReplicates = true` 写在 `BeginPlay` → 已经太晚，初始复制握手期错过了
