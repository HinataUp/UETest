# T10-T13 指针 / 引用复制

## 模块总览
同步指针和同步 int 在机制上完全不同。指针在网络上**不是地址，是网络对象引用（NetGUID）**。Client 收到后要在本地查找对应的对象。本目录把四种指针场景走一遍，建立"什么能复制、什么不能、为什么"的心理模型。

| Task | 主题 | Actor |
|---|---|---|
| T10 | 关卡中已存在的 Actor 引用（最简单）| `AT10_LevelActorRefActor` |
| T11 | Server 运行时 Spawn 的 Actor 引用（Unmapped 现象）| `AT11_SpawnedActorRefActor` |
| T12 | 普通 UObject 引用（失败示例）| `AT12_UObjectRefActor` |
| T13 | DefaultSubobject Component 引用 | `AT13_ComponentRefActor` |

完成后你能回答：
- 为什么 Server 同帧 Spawn 并赋值的 Actor 指针，Client OnRep 时可能是 null？
- 普通 UObject 能不能被 Replicated 指针同步？为什么？怎么解决？
- DefaultSubobject 与运行时 AddComponent 在复制上有什么区别？

## 使用方法

**T10：**
1. 关卡里放两个 Actor：一个 `AT10_LevelActorRefActor`、一个普通 StaticMeshActor（叫 Target1）
2. 选中 T10 Actor → Details 面板 → `T10_TargetInLevel` → 拖入 Target1
3. PIE → 2 秒后 OnRep 触发，三端打 `OnRep_TargetActor=Target1`

**T11：**
1. 关卡放一个 `AT11_SpawnedActorRefActor`
2. Details 设置 `T11_SpawnClass = ANetTestActor`（或任意 bReplicates Actor 类）
3. PIE → 3 秒后 Server Spawn 一个 → 偶尔能在 Client 日志里看到 `OnRep_SpawnedRef=null` 后又重新触发解析

**T12：**
1. 关卡放一个 `AT12_UObjectRefActor`
2. PIE → Server 端 5 秒后日志显示 `InfoRef=PlainInfoObject_X`，Client 端永远显示 `nullptr`

**T13：**
1. 关卡放一个 `AT13_ComponentRefActor`
2. PIE → 2 秒后三端都正确解析 `RepMeshRef=T13_MeshComp`

## 实现要点

**TObjectPtr vs 裸指针：**
```cpp
UPROPERTY(Replicated) AActor* Ptr;            // UE4 风格
UPROPERTY(Replicated) TObjectPtr<AActor> Ptr; // UE5 推荐，复制行为等价
```

**容错处理 Unmapped：**
```cpp
void OnRep_TargetActor()
{
    if (TargetActor)
    {
        // 解析成功
    }
    else
    {
        // 第一次可能是 null —— 不是 bug，UE 会等被引用 Actor 到达后再次触发 OnRep
        // 工程做法：用 Timer 或下次 OnRep 兜底，不要假设第一次就拿到
    }
}
```

## 原理简述

**复制的不是地址，是 NetGUID：**
- 每个参与复制的对象（Actor / DefaultSubobject）有稳定的 NetGUID
- Server 发的"指针"实际上是 NetGUID 数字
- Client 收到后查表：本地有这个 NetGUID 吗？有 → 解析成功；没有 → Unmapped Reference 挂起

**为什么场景里的 Actor 容易解析：**
- Client 加载关卡时本地构造了同一个 Actor，NetGUID 在两端一致
- 所以引用立即解析成功

**为什么 Spawn Actor 可能 null：**
- Server 同帧：Spawn ActorB → 赋值 ActorA.Ref = ActorB
- Client 收 ActorA 复制时，ActorB 的复制可能还在 ActorA 之后到达
- UE 此时挂起引用为 Unmapped，并在 ActorB 到达后**再次触发 OnRep**

**为什么普通 UObject 失败：**
- 普通 UObject 没 NetGUID，Client 上根本没有对应实例
- 解决方案：
  - **改成 Actor**（最直接）
  - **`ReplicatedSubObject`**（GAS 走的路线，见 T26）
  - **结构体复制内部数据**，两端各自 NewObject 影子

**Component 为什么能复制：**
- `CreateDefaultSubobject` 创建的 Component 也有 NetGUID
- Client 加载 Actor 时同步构造同名 Component，NetGUID 在两端对齐
- 运行时 `AddComponent` 创建的 Component 没自动获得 NetGUID，需要额外配置

## 测试方法

**手动 PIE：**
- T10：稳定，每次 OnRep 都解析成功
- T11：可能需要多跑几次才能复现 null 现象——网络快时 ActorB 几乎与 ActorA 同时到达
- T12：Server 永远非空，Client 永远 null（这就是预期）
- T13：稳定，每次都成功

**自动化测试：** 本组的"跨端解析"行为单测无法直接验证（需要真实网络层）。属性注册可以由通用的 RepProps 检查覆盖：

```
UETest.Network.属性复制 中没有为 T10-T13 单独写
但你可以在 PropertyReplicationSpec.cpp 仿照 T1-T5 加几个：
  TestTrue("T10_TargetActor 已注册",
      ContainsReplicatedProp(Props, AT10_LevelActorRefActor::StaticClass(),
          TEXT("T10_TargetActor")));
```

## 常见坑
- 引用一个 `bReplicates=false` 的 Actor → Client 永远 null
- 在 OnRep 里假设 `TargetActor` 一定非空 → 偶发 nullptr crash
- 用 `TWeakObjectPtr` 做 Replicated 字段 → 不参与复制，只是本地弱引用
- 误以为 UObject 能跨端 → 改成 Actor 或走 ReplicatedSubObject
