# B 章节 - AI 系统 (P70-P77)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P70 | BehaviorTree Task / Decorator / Service | `P70_BTTask_FindTarget` + `P70_BTDecorator_IsInRange` |
| P71 | EQS Environment Query System | 见下方"P71 步骤" |
| P72 | AI Perception | `P72_PerceptionAIController.h/cpp` |
| P73 | NavMesh + PathFollowing | `P73_NavMeshHelper.h/cpp` |
| P74 | StateTree（UE5 新 AI 系统）| 见下方"P74 笔记" |
| P75 | Smart Objects | 见下方"P75 笔记" |
| P76 | Mass Entity（ECS）| 见下方"P76 笔记" |
| P77 | Mass Spawner + ISM Crowd | 见下方"P77 笔记" |

## P70 - Behavior Tree

**三种节点：**
| 节点 | 何时触发 | 用途 |
| --- | --- | --- |
| Task | 执行业务（攻击 / 寻路）| 返回 Succeeded / Failed / InProgress |
| Decorator | 进入子树前判定 | 条件分支 |
| Service | 子树激活期间周期 Tick | 持续更新 Blackboard |

**编辑器配 BT：**
1. New → AI → Behavior Tree（绑定 Blackboard 资产）
2. 拖入节点 ：Selector / Sequence / 自定义 P70 Task / Decorator
3. 在 Details 设 Blackboard Key Selector

**AIController 启动 BT：**
```cpp
RunBehaviorTree(MyBehaviorTree);   // 自动读取 BT 上绑的 Blackboard
```

## P71 - EQS（Environment Query System）

**启用：** Project Settings → AI System → Generate Navigation Only Around Navigation Invokers / EQS Testing Pawn

**编辑器步骤：**
1. New → AI → Environment Query
2. Generator ：Points_Donut（生成圆环点）/ ActorsOfClass
3. Tests ：Distance / Trace / PathLength（每个 Test 给点打分）
4. EQS Testing Pawn ：场景拖入 → 选 Query → 可视化查询结果

**C++ 执行：**
```cpp
FEnvQueryRequest Req(EnvQuery, Querier);
Req.Execute(EEnvQueryRunMode::SingleResult,
    FQueryFinishedSignature::CreateLambda([](TSharedPtr<FEnvQueryResult> R) { /* 用结果 */ }));
```

**集成 BT：** BT 节点 `BTTask_RunEQSQuery` ：直接在 BT 中调用 EQS。

## P72 - AI Perception

```cpp
PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
SightConfig->SightRadius = 1500.f;
PerceptionComp->ConfigureSense(*SightConfig);
PerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());
PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &::HandleTargetPerceptionUpdated);
```

**关键：**
- `OnPerceptionUpdated` ：广播所有感知变化（多个 actor）
- `OnTargetPerceptionUpdated` ：每个 actor 单独触发（推荐 ；含 Stimulus 详情）
- Project Settings → AI System → Sight Sense → 配置阵营关系

## P73 - NavMesh

**编辑器步骤：**
1. 关卡放 NavMeshBoundsVolume → 包住可走区域
2. P 键预览 NavMesh
3. Build → Build Paths

**移动 API：**
```cpp
AICon->MoveToActor(Target, AcceptanceRadius);
AICon->MoveToLocation(Location, AcceptanceRadius);
```

**进阶：**
- `NavModifierVolume` ：标记某区域为高代价（让 AI 绕行）/ 不可通行
- `NavLinkProxy` ：让 AI 跳过间隙 / 攀爬

```cpp
UP73_NavMeshHelper::MoveAIToActor(AICon, Target);
UP73_NavMeshHelper::GetRandomNavLocation(this, Origin, 500.f, OutLoc);
```

## P74 - StateTree（UE5 新 AI 系统）

**Plugin：** StateTree ；UE5.5+ 已成熟。

**与 BT 对比：**
| 维度 | BT | StateTree |
| --- | --- | --- |
| 状态切换 | 全树重新评估 | 显式 Transition |
| 数据流 | 通过 Blackboard | Property Bindings 直接绑 |
| 并行 | 困难 | 原生支持 |
| 编辑器 | 节点图 | 状态图 + 标签 |

**步骤：** New → StateTree → Schema 选 `UStateTreeComponentSchema`

**自定义 Task：**
```cpp
USTRUCT() struct FMyTask : public FStateTreeTaskBase {
    GENERATED_BODY()
    virtual EStateTreeRunStatus EnterState(...) const override;
    virtual EStateTreeRunStatus Tick(...) const override;
};
```

UE 官方建议 ：新项目优先 StateTree ；BT 适合简单任务级 AI。

## P75 - Smart Objects

**Plugin：** SmartObjects（UE5）

**数据：** USmartObjectDefinition 资产 ：定义"坐椅子"行为（Tag + 动画 + Slot）

**用法：**
```cpp
USmartObjectSubsystem* Sub = USmartObjectSubsystem::GetCurrent(World);
FSmartObjectClaimHandle Claim = Sub->Claim(Component);
Sub->Use(Claim);
```

**集成 BT/StateTree：** UseSmartObject Task 节点。

## P76 - Mass Entity（ECS）

**Plugin：** MassEntity / MassGameplay

**核心：**
| 类型 | 用途 |
| --- | --- |
| `FMassFragment` | 数据（Position / Velocity / Health）|
| `FMassTag` | 标记位（IsEnemy / IsDead）|
| `UMassProcessor` | 逻辑（每帧遍历有 Fragment X 的实体）|
| `FMassEntityQuery` | 查询条件 |

```cpp
USTRUCT() struct FHealthFragment : public FMassFragment {
    GENERATED_BODY() float Health = 100.f;
};

UCLASS() class UMovementProcessor : public UMassProcessor {
    virtual void ConfigureQueries() override;
    virtual void Execute(FMassEntityManager&, FMassExecutionContext&) override;
};
```

**适用：** 大规模实体（千级 NPC / 弹幕 / 鸟群）；性能远超 Actor。

## P77 - Mass Spawner

**配套：** MassRepresentation ：远处 ISM 渲染 ，近处切换 Actor。

**步骤：**
1. AMassSpawner 放关卡 ；Entity Config 指定 Fragment 集合
2. RepresentationSubsystem 自动按距离切换 ISM / Actor
3. `stat unit` / `stat ism` 看 ISM 实例数动态变化

## 测试方法

**自动化测试：** `UETest.Fundamentals.P3_UI_AI_动画 → B_AI`
```
P70 BTTask_FindTarget 是 BTTaskNode 子类
P70 BTDecorator 默认 Range 合理
P72 PerceptionComp + SightConfig 已创建
P73 NavMeshHelper 是 BlueprintFunctionLibrary
```

**手动 PIE：**
- P70 ：搭 BT + Blackboard ；AIController RunBehaviorTree → AI 巡逻 → 找目标 → 接近 → 攻击
- P71 ：EQS Testing Pawn → 可视化绿/红球分布
- P72 ：玩家走入视野 → Output Log 看 "P72: Perception X Sensed"
- P73 ：场景配 NavMesh → AICon->MoveToActor 看 AI 寻路绕障
- P74-P77 ：见各自插件文档

## 常见坑
- BT Decorator 配在 Composite 节点上 vs 配在子节点上 ：判定时机不同（Composite 进入前 vs 子节点 Tick 时）
- AIPerception 的"阵营关系"需要 IGenericTeamAgentInterface 实现
- NavMeshBoundsVolume 不包住玩家起点 → AI 寻路失败
- StateTree 与 BT 不能简单互转 ；选型时考虑团队熟悉度
- Mass Entity Fragment 大小需对齐 ；不规范字段会触发 ensure
