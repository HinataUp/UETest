# B 章节 - Gameplay 框架全家族 (P35-P40)

## 模块总览

| Task | 主题 | 文件 |
| --- | --- | --- |
| P35 | GameInstance（跨关卡持久）| `P35_LifecycleGameInstance.h/cpp` |
| P36 | GameMode + GameState | `P36_LifecycleGameMode.h/cpp` |
| P37 | PlayerController + PlayerState | `P37_LifecyclePlayerController.h/cpp` |
| P38 | Pawn / Character / Possess | `P38_39_LifecycleCharacter.h/cpp` |
| P39 | Enhanced Input | 同上（合并）|
| P40 | 完整框架巡礼 | 见本 README"完整初始化链路"|

## 一图看懂家族关系

```
UGameInstance（进程内唯一）
   └─ 每个 World 一个 ：
       ├─ AGameModeBase（Server only，规则/Spawn/Match）
       ├─ AGameStateBase（Replicated，全局状态）
       ├─ APlayerController（每端一个，输入/UI/Camera）
       │    └─ 控制 APawn / ACharacter
       └─ APlayerState（Replicated，玩家数据，跨 Pawn 持久）
```

## P35 - GameInstance

**定位：** 进程内唯一 ；关卡切换不销毁 ；适合放跨关卡持久数据（玩家档案 / 在线服务引用 / 全局设置）。

**配置：** Project Settings → Maps & Modes → Game Instance Class

```cpp
class UMyGI : public UGameInstance {
    virtual void Init() override;       // 进程启动唯一一次
    virtual void Shutdown() override;   // 进程退出
    UPROPERTY() FString PlayerProfileName;   // 关卡切换不丢
};
```

## P36 - GameMode + GameState

**关键边界：**
| 类 | 在哪 | 用途 |
| --- | --- | --- |
| GameMode | **Server only** | 规则 / Spawn / 鉴权 |
| GameState | **所有端 Replicated** | 全局共享状态 |

```cpp
// Server 端
AMyGameMode* GM = GetWorld()->GetAuthGameMode<AMyGameMode>();   // 非空
// Client 端
AMyGameMode* GM = GetWorld()->GetAuthGameMode<AMyGameMode>();   // nullptr ！

// 两端都能拿
AMyGameState* GS = GetWorld()->GetGameState<AMyGameState>();
```

## P37 - PlayerController + PlayerState

**PC 在哪：**
- Server 端 ：每个连入玩家有一个
- OwningClient 端 ：自己有一个
- 别的 Client 看其他玩家：拿不到他的 PC

**PS 在哪：** 所有端，Replicated。所以"读别人的分数"用 `PlayerState`。

**Possess 时序：**
```
[Server] PossessedBy(Controller) ─────────┐
                                          ├─ 复制 Controller 字段
[Client] OnRep_Controller ────────────────┘
```

## P38 + P39 - Character + Enhanced Input

**ACharacter = APawn + Capsule + CharacterMovementComp + SkeletalMesh**。比 APawn 多一套现成的角色动作系统。

**Enhanced Input 三件：**
```cpp
// 1. UInputAction (Axis2D / Bool)
// 2. UInputMappingContext (键位映射 + Modifier + Trigger)
// 3. C++ BindAction
EIC->BindAction(IA_Move,   ETriggerEvent::Triggered, this, &::OnMove);
EIC->BindAction(IA_Attack, ETriggerEvent::Started,   this, &::OnAttackStart);
EIC->BindAction(IA_Attack, ETriggerEvent::Completed, this, &::OnAttackComplete);
```

**ETriggerEvent 五种：**
- `Started` ：按下瞬间
- `Triggered` ：满足触发条件期间持续触发
- `Ongoing` ：处于触发中（与 Triggered 互补）
- `Completed` ：自然结束（释放 / Timer 到 ）
- `Canceled` ：被取消

## P40 - 完整初始化链路

**Listen Server / Client 启动时的顺序：**
```
GameInstance::Init                       (进程级)
  → GameMode::InitGame                   (Server)
  → GameMode::PreLogin
  → GameMode::Login
  → PlayerController::BeginPlay
  → GameMode::PostLogin
  → GameMode::HandleStartingNewPlayer
  → GameState::BeginPlay
  → PlayerState::BeginPlay
  → Character::PossessedBy
  → Character::BeginPlay
```

跑 `AP31` / `AP35` / `AP36` / `AP37` / `AP38` 这一套类（GameMode 配 BP 子类绑相应 ClassDefault），观察 Output Log 的日志顺序，与上面对照。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → B_Gameplay框架`
```
P36 GameMode.GameStateClass 绑定
P36 GameState bReplicates + MatchTimer 在 RepProps 中
P37 PlayerState bReplicates
P38 Character 暴露 IA_Move 字段
```

**手动 PIE：**
- 把 GameMode / PlayerController / PlayerState / Character 的 BP 子类绑到 World Settings → 启动 → Output Log 看完整链路日志

## 常见坑
- Client 端调 `GetAuthGameMode` 拿到 nullptr → 这是预期；要全局状态用 GameState
- 想跨关卡保存数据放 GameMode 里 → 错；GameMode 关卡切换会销毁 ；放 GameInstance
- Enhanced Input 没把 MappingContext Add 到 Subsystem → Action 不触发
- Enhanced Input 把 BindAction 写在 BeginPlay 而不是 SetupPlayerInputComponent → InputComponent 还没就绪 ，绑定丢失
