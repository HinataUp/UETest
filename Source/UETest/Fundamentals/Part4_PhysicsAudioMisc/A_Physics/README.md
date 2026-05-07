# A 章节 - 物理与碰撞 (P86-P88)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P86 | Collision Channel + Response 矩阵 | `P86_CollisionShowcaseActor.h/cpp` |
| P87 | Line / Sweep / Overlap Trace | `P87_TraceHelper.h/cpp` |
| P88 | Physics Simulation + Force / Impulse | `P88_PhysicsCubeActor.h/cpp` |

## P86 - Collision

**Project Settings → Collision：**
- **Object Channels**：自定义 ECC_GameTraceChannel1..18 ；如 `Projectile`
- **Trace Channels**：用于 LineTrace / Sweep
- **Profiles**：预设响应矩阵 ；BP 中下拉选

**代码层 ：**
```cpp
SphereComp->SetCollisionProfileName(FName("BlockAllDynamic"));   // 用预设
// 或手动
SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
SphereComp->SetGenerateOverlapEvents(true);
```

**响应规则（Block vs Overlap vs Ignore）：**
| 双方设置 | 结果 |
| --- | --- |
| 两边都 Block | OnHit 触发 ；停下 |
| 至少一方 Overlap（无 Block）| OnOverlap 触发 ；穿透 |
| 任一方 Ignore | 无事件 |

## P87 - Trace

```cpp
// Line Trace
FHitResult Hit;
GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

// Sphere Sweep ：Shape 沿 Start→End 扫
FCollisionShape Shape = FCollisionShape::MakeSphere(50.f);
GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Pawn, Shape, Params);

// Overlap Multi ：找 Origin 周围所有
TArray<FOverlapResult> Out;
GetWorld()->OverlapMultiByChannel(Out, Origin, FQuat::Identity, ECC_Pawn, Shape, Params);
```

**FCollisionQueryParams：** AddIgnoredActor / bTraceComplex / bReturnPhysicalMaterial。

**Debug Draw：** `DrawDebugLine` / `DrawDebugSphere`（开发期可视化）；`#if ENABLE_DRAW_DEBUG` 包裹避免 Shipping 编译。

## P88 - Physics Simulation

```cpp
CubeMesh->SetSimulatePhysics(true);                 // 受重力
CubeMesh->AddImpulse(Direction * 5000.f);           // 瞬时冲量
CubeMesh->AddForce(Wind);                           // 持续施力（每 Tick）
CubeMesh->SetPhysMaterialOverride(MyPhysicsMat);    // 摩擦 / 弹性
```

**PhysicsMaterial 关键字段：**
- `Friction` ：摩擦系数（0-1）
- `Restitution` ：弹性（0=不弹 / 1=完全弹）
- `Density` ：密度（影响质量）

**Physics Constraint：** 编辑器 PhysicsConstraintActor 或代码 `UPhysicsConstraintComponent` ；连接两 Mesh 形成铰链 / 弹簧。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P4_物理音频特效 → A_物理碰撞`
```
P86 SphereComp 默认配置（ObjectType / GenerateOverlap）
P87 TraceHelper 是 BP Function Library
P88 物理 Cube SimulatePhysics + Movable + Impulse > 0
```

**手动 PIE：**
- P86 ：拖入 P86 Actor + 一个带 Pawn ProfileMesh ；走过去看 OnHit 日志 ；走过 WorldDynamic 看 OnOverlap
- P87 ：BP 调 LineTraceVisibility(bDrawDebug=true) 看红 / 绿线
- P88 ：拖 P88 Actor 到关卡 → 它会因重力下落 → BP 调 ApplyImpulse 看弹飞

## 常见坑
- `SetGenerateOverlapEvents(false)` → OnOverlap 永不触发（默认 false）
- 自定义 Channel 名字编辑器中改 → 已存的资产 ECC 索引可能错位 ；改名前先确认引用
- LineTrace 被自身 Mesh 命中 → 加 `Params.AddIgnoredActor(this)`
- AddImpulse 第三参 bVelChange ：true=忽略质量直接改速度（爆炸式）；false=按质量加冲量
- 动态物体 SetMobility 必须 Movable ；构造期 Static 默认不动
