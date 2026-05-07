# H 章节 - Networking + Level Streaming (P59-P60)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P59 | 网络复制基础 综合示例 | `P59_NetworkBasicsActor.h/cpp` |
| P60 | World Partition + Level Streaming + ServerTravel | `P60_LevelStreamingHelper.h/cpp` |

> 注 ：网络复制完整覆盖 30 个 task 已在 `Source/UETest/NetworkReplication/` 模块。本章节只综合一个 Actor 演示。

## P59 - 网络复制综合示例

```cpp
// Replicated 字段 + OnRep
UPROPERTY(ReplicatedUsing = OnRep_Health) int32 Health = 100;

// 三种 RPC
UFUNCTION(Server, Reliable, WithValidation) void ServerRequestDamage(int32 Amount);
UFUNCTION(Client, Reliable)                 void ClientShowMessage(const FString& Msg);
UFUNCTION(NetMulticast, Unreliable)         void MulticastPlayHitFX(FVector Location);

// GetLifetimeReplicatedProps
DOREPLIFETIME(AP59_NetworkBasicsActor, Health);
```

**判断当前端：**
```cpp
HasAuthority()                                       // Server
GetLocalRole() == ROLE_Authority / Autonomous / Simulated
```

## P60 - World Partition + Streaming

### World Partition（UE5 推荐 ：大世界）
- 编辑器配置 ：World Settings → World Partition Setup
- 自动按 Runtime Grid 加载/卸载
- 不需要写代码 ；Data Layer 可代码控制（隐藏装饰物 / 难度模式切换）

### 手动 Level Streaming
```cpp
UP60_LevelStreamingHelper::LoadStreamLevelByName(
    this, FName(TEXT("L_Sublevel_Town")),
    /*bMakeVisibleAfterLoad=*/true,
    /*bShouldBlock=*/false);
```

**回调：** 通常用 `LatentInfo + LatentAction` ；蓝图中显示为带"OnLoaded"引脚的节点。

### Server Travel
```cpp
World->ServerTravel("/Game/Maps/L_Battle");
```
- 所有客户端跟随
- 想 Seamless 切换：GameMode 设 `bUseSeamlessTravel=true` + 在 Project Settings 配 TransitionMap
- ServerTravel 只能在 Server 上调用

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → H_网络与关卡`
```
P59 NetworkBasicsActor RPC 标记正确（FUNC_NetServer/Client/Multicast）
```

**手动 PIE：**
- P59 ：Listen Server + 1 Client → Server 端调 `ServerRequestDamage(20)` → 三端 OnRep_Health 触发
- P60 ：放 World Partition 关卡 → 走到远处看 Runtime Grid 自动加载/卸载 ；调 `LoadStreamLevelByName` 看子关卡显隐

## 常见坑
- Client 调 ServerTravel → 无效（Server only）
- LoadStreamLevel 但子关卡未在 Persistent Level 的 Levels 列表里 → 失败
- 把 ServerTravel 用于单机模式 → 等价 OpenLevel + 玩家重连
- WithValidation 函数 _Validate 返回 false → 玩家被踢；只用于"明显作弊"
- World Partition 关卡用 `OpenLevel` 加载 → 错；用 `LoadLevel` 或 World Partition 子系统 API
