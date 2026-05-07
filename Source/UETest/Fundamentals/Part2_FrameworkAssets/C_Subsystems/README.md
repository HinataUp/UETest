# C 章节 - Subsystem + Tag + 消息 (P41-P43)

## 模块总览

| Task | 主题 | 文件 |
| --- | --- | --- |
| P41 | 四种 Subsystem 对比 | `P41_AllSubsystems.h/cpp` |
| P42 | GameplayTag 系统（Channel Tag）| `P42_NativeTagsP2.h/cpp` |
| P43 | GameplayMessageSubsystem | `P43_DamageMessage.h` |

## P41 - 四种 Subsystem 选型

| 类型 | 生命周期 | 用途 |
| --- | --- | --- |
| `UEngineSubsystem` | 进程 | 全局工具 / 编辑器扩展 |
| `UGameInstanceSubsystem` | GameInstance | **跨关卡的全局服务**（最常用）|
| `UWorldSubsystem` | World | 关卡内系统（关卡切换销毁重建）|
| `ULocalPlayerSubsystem` | 每个本地玩家 | UI / 输入 / 个人状态 |

**访问：**
```cpp
GEngine->GetEngineSubsystem<UMyEngineSub>();
GameInstance->GetSubsystem<UMyGameInstSub>();
World->GetSubsystem<UMyWorldSub>();
LocalPlayer->GetSubsystem<UMyLocalPlayerSub>();
```

**控制创建：** override `ShouldCreateSubsystem(Outer)` 返回 false 可阻止某些情况下创建。`UWorldSubsystem` 还可 override `DoesSupportWorldType(WorldType)` 限制只在 Game/PIE 创建。

## P42 - GameplayTag

复用网络同步 / GAS 模块已有的 Native Tag 注册模式 ；本章只补充 P43 用的 Channel Tag：
```cpp
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Damage_Dealt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Player_Joined);
```

## P43 - GameplayMessageSubsystem

**插件依赖：** `Plugins → Gameplay Message Router`（Lyra 同款）。启用后包含 `GameplayMessageSubsystem.h` 即可：

```cpp
// 发布
FP43_DamageMessage Msg;
Msg.Target = Target; Msg.Amount = 50.f;
UGameplayMessageSubsystem& MsgSys = UGameplayMessageSubsystem::Get(WorldContext);
MsgSys.BroadcastMessage(P2_Tags::TAG_Message_Damage_Dealt, Msg);

// 订阅
FGameplayMessageListenerHandle H = MsgSys.RegisterListener<FP43_DamageMessage>(
    P2_Tags::TAG_Message_Damage_Dealt,
    [](FGameplayTag Channel, const FP43_DamageMessage& M) {
        UE_LOG(LogTemp, Log, TEXT("收到伤害 %f"), M.Amount);
    });

// 取消
MsgSys.UnregisterListener(H);
```

**用途：** 跨系统解耦通信。HUD / 成就 / 日志各自独立订阅同一 Channel ，互不知晓。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → C_Subsystems与Tag`
```
P41 四个 Subsystem 类反射可达
P42 Tag Native 注册成功
P43 DamageMessage struct 字段
```

**手动 PIE：**
- 启 PIE → Output Log 看 `P41: WorldSubsystem::Initialize` ；切关卡看到 Deinit + Re-Init
- GameInstanceSubsystem 不会因关卡切换销毁

## 常见坑
- WorldSubsystem 期望"跨关卡保留" → 错；用 GameInstanceSubsystem
- GameInstanceSubsystem 期望访问 World → 关卡切换瞬间 World 可能为空，要 ensure(GetWorld())
- LocalPlayerSubsystem 在 Dedicated Server 上不存在
- GameplayMessage 插件未启用 → 编译错；Project Settings → Plugins 启用 "Gameplay Message Router"
