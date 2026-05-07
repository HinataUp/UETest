# T16-T20 RPC 变体

## 模块总览
RPC（Remote Procedure Call）是属性复制之外的另一根同步支柱。属性复制走"状态同步"，RPC 走"事件同步"——一次性触发，不保证后续可查询。

| Task | 主题 | Actor |
|---|---|---|
| T16 | Server RPC（Reliable / Unreliable）| `AT16_ServerRPCPawn` |
| T17 | Client RPC（私有反馈）| `AT17_ClientRPCPawn` |
| T18 | Multicast RPC（群体广播）| `AT18_MulticastRPCActor` |
| T19 | RPC 参数类型（值 / 结构体 / 指针 / 数组）| `AT19_RPCParamsActor` |
| T20 | `_Validate` 校验 | `AT20_RPCValidationPawn` |

完成后你能回答：
- Server / Client / Multicast 三种 RPC 的区别？
- Reliable vs Unreliable 怎么选？
- 为什么 Server RPC 需要 Owner？
- `_Validate` 什么时候返回 false？

## 使用方法

**T16 / T20（需要被 Possess）：**
1. 设置 GameMode 的 Default Pawn Class = `AT16_ServerRPCPawn` 或 `AT20_RPCValidationPawn`
2. 三窗口 PIE → 各 Client 自己的 Pawn 每秒触发一次 RPC
3. 用 `net.PktLoss 30` 模拟丢包，对比 Reliable / Unreliable 行为

**T17：**
1. 设置 Default Pawn Class = `AT17_ClientRPCPawn`
2. 观察：Server 每 3 秒发一次，**只有 Owner 客户端**收到日志

**T18：**
1. 关卡里放 `AT18_MulticastRPCActor`
2. 观察：Server / 所有 Client 都打 `MulticastVFX` 日志

**T19：**
1. 关卡放 `AT19_RPCParamsActor` → 4 秒一次综合参数广播

## 实现要点

**RPC 三件套：**
```cpp
// 头文件：声明
UFUNCTION(Server, Reliable, WithValidation)
void ServerDoSomething(int32 Param);

// 实现：UHT 自动生成"分发函数"，你只实现 _Implementation 和 _Validate
void MyClass::ServerDoSomething_Validate(int32 Param) { return Param >= 0; }
void MyClass::ServerDoSomething_Implementation(int32 Param) { /* 业务 */ }
```

**三种 RPC 标记速查：**
| 标记 | 调用方 | 执行方 |
|---|---|---|
| `UFUNCTION(Server, ...)` | Client | Server |
| `UFUNCTION(Client, ...)` | Server | Owner Client |
| `UFUNCTION(NetMulticast, ...)` | Server | Server + 所有相关 Client |

**Reliable vs Unreliable：**
- Reliable：保证到达 + 按顺序，但占可靠通道带宽，溢出会踢下线
- Unreliable：丢了就丢，开销小，适合高频小事件

**_Validate 边界（重要）：**
```cpp
bool MyClass::ServerFire_Validate(int32 Damage)
{
    // ✓ 拦截"明显作弊"：参数越界 / 字符串过长 / 数组过大
    return Damage >= 0 && Damage <= MaxAllowedDamage;
}

void MyClass::ServerFire_Implementation(int32 Damage)
{
    // ✓ 业务规则放这里：冷却、资源、距离
    if (bIsOnCooldown) { return; }
    // ...
}
```
- _Validate 返回 false → **断开 Client 连接**，是严重措施
- 不要用 _Validate 做业务规则，会导致玩家因正常游戏被踢

## 原理简述

**Server RPC 的传递路径：**
```
Client.Pawn.ServerRPC(args)
    → UE 序列化 args → 发包到 Server
    → Server 端：调用 ServerRPC_Validate(args)
       false → 断开连接
       true  → 调用 ServerRPC_Implementation(args)
```

**Client RPC 走 Owner Connection：**
- Server 上调 `Pawn.ClientRPC()`
- UE 查 `Pawn.GetNetConnection()`（即 Owner.PlayerController.NetConnection）
- 包发给该 Connection；其他 Connection 收不到
- 没 Owner 的 Actor 上调 Client RPC → 静默丢弃 + warning

**Multicast 的相关性：**
- 不是发给"所有 Client"，而是发给"能感知到这个 Actor"的 Client
- UE 的 Relevancy 系统按距离 / Cull 决定可见性
- 这是 Multicast 在大世界里依然能扩展的关键

**为什么 Server RPC 必须有 Owner：**
- Server 必须知道这个 RPC 来自哪个 Connection（鉴权）
- Owner 链给出 Connection 身份
- 在没 Owner 的场景 Actor 上调 Server RPC → UE 拒绝

## 测试方法

**手动 PIE：**
- T16：开 `net.PktLoss 30`，Reliable 计数完整、Unreliable 缺失
- T17：只有 Owner 客户端窗口看到 `ClientRecv Msg=...`
- T18：所有窗口都看到 `MulticastVFX`
- T19：Multicast 包成功传递结构体、数组、Actor 引用
- T20：Damage=99999 时连接被断开，前端控制台报错

**自动化测试：** `UETest.Network.RPC`

```
覆盖：
  T16 ServerSendReliable 必须带 FUNC_NetServer + FUNC_NetReliable
     ServerSendUnreliable 不应带 FUNC_NetReliable
  T17 ClientShowMessage 必须带 FUNC_NetClient
  T18 MulticastPlayVFX 必须带 FUNC_NetMulticast
  T20 _Validate 校验逻辑
     - 合法参数 (10) 应通过
     - 过大参数 (99999) 应被拒绝
     - 负数应被拒绝
     - UFUNCTION 必须带 FUNC_NetValidate
```

## 常见坑
- 实现了不带后缀的版本（`ServerFire`）→ 与 UHT 自动生成的代码冲突；必须实现 `_Implementation`
- 在没 Owner 的场景 Actor 上调 Server RPC → 静默忽略 + warning
- 用 Reliable Multicast 高频广播 → 极易撑爆带宽，建议 Unreliable
- `_Validate` 用作业务规则判断 → 玩家被错误断线
- RPC 参数过大（大数组 / 长字符串）→ 超 MTU 会出错，改用属性复制
