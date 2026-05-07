# 00_Common - GAS 公共基础设施

## 模块总览
所有章节共用的底座，集中放在这里以避免循环依赖：

| 文件 | 用途 |
| --- | --- |
| `GASLogMacros.h/cpp` | `GAS_LOG / GAS_VLOG` 统一日志，分类 `LogGASTest` |
| `MyGameplayTags.h/cpp` | A04 - Native Tag 注册（被 B/C/D/E/F/G/H/I 引用）|
| `GASTestPlayerState.h/cpp` | 玩家用 ASC 容器（Mixed 复制模式，PlayerState 持有）|
| `GASTestAICharacter.h/cpp` | AI 用 ASC 容器（Minimal 复制模式，Character 持有）|
| `GASTestActor.h/cpp` | 测试 / 打靶用极简 Actor（NewObject 即得）|

## 使用方法

**手动 PIE：**
- 玩家角色 ：把 GameMode 的 `PlayerStateClass` 设为 `AGASTestPlayerState`
- AI 靶子 ：直接拖 `AGASTestAICharacter` 或 `AGASTestActor` 到关卡
- Tag ：代码中通过 `MyGameplayTags::TAG_xxx` 直接引用

**自动化测试：**
- `Tests/GASTestSpecUtils.h` 提供 `FTestEnv` ：一行 `Setup()` 拿到 ASC + HealthSet + CombatSet

## 实现要点

**Player ASC 在 PlayerState 而不是 Pawn：**
- PlayerState 跨 Pawn 持久（重生不重置 ASC）
- Mixed 复制模式要求 ASC.OwnerActor 是 PlayerState ；放在 Pawn 上 Mixed 模式会异常

**AI ASC 在 Character 自身：**
- AI 没 PlayerState 概念（AIController 的 PlayerState 通常 null）
- Minimal 复制模式开销小，适合大量 NPC

**InitAbilityActorInfo 时机：**
```cpp
void AGASTestPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
    // 第一参 OwnerActor ：拥有 ASC 的 Actor（PlayerState）
    // 第二参 AvatarActor：实际的 Pawn
}
```

## 原理简述

**GAS 的 OwnerActor / AvatarActor 解耦：**
- OwnerActor ：ASC 真正的所有者，决定数据生命周期
- AvatarActor ：Avatar Pawn ，决定碰撞 / 动画 / 输入交互
- 重生时 Pawn 销毁 ，OwnerActor 不变，ASC 内的属性 / GE / GA 全部保留

**Replication Mode：**
| 模式 | 适用 | 复制内容 |
|---|---|---|
| Full | 不推荐多人 | 全部 GE 详细信息广播给所有人 |
| Mixed | 玩家 | Owner 收完整 GE，Simulated 只收 Tag/属性 |
| Minimal | AI | 只复制必要 Tag/属性 ，省带宽 |

## 测试方法

**自动化测试：** 此目录的代码在每个章节的 Spec 中被 `FTestEnv::Setup()` 间接覆盖，不单独测。

**手动 PIE：**
1. GameMode 设 `AGASTestPlayerState` 为 PlayerStateClass
2. PIE 启动后，控制台 `showdebug abilitysystem` 应能看到 ASC 初始化完成
3. 应能看到 HealthSet/CombatSet 默认值（100/100）

## 常见坑
- 把 ASC 创建在 Pawn 上后改用 Mixed 模式 → Simulated Proxy 收不到 Tag
- 忘了 `SetIsReplicated(true)` → ASC 状态完全不同步
- `InitAbilityActorInfo` 在 BeginPlay 而不是 PostInitializeComponents → 初始化竞态
