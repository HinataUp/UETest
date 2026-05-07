# A 章节 - Actor / Component 架构 (P31-P34)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P31 | Actor 完整生命周期 | `P31_LifecycleActor.h/cpp` |
| P32 | Component 创建 / 注册 / 销毁 | `P32_ComponentManagementActor.h/cpp` |
| P33 | Spawn 三种方式 | `P33_SpawningHelper.h/cpp` |
| P34 | Component 通信查找 | `P34_ComponentLookupActor.h/cpp` |

## 关键认知

**P31 - Actor 生命周期：**
```
Constructor          ：不能访问 World
PostInitializeComponents ：DefaultSubobject 都就绪 ；可以访问 World
BeginPlay            ：所有 Actor 都准备好 ；最安全的初始化点
Tick                 ：每帧
EndPlay              ：清理 Timer / 解绑委托
Destroyed            ：彻底销毁前最后一击
```

**P32 - 组件创建二选一：**
```cpp
// 构造期 ：唯一合法 API
RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

// 运行时 ：必须三步走
UStaticMeshComponent* C = NewObject<UStaticMeshComponent>(this);
C->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
C->RegisterComponent();   // 缺这步 ：不 Tick / 不渲染 / 不物理
```

**P33 - Spawn 三态：**
```cpp
// 标准 ：Spawn + BeginPlay 立即触发
AActor* A = World->SpawnActor<AActor>(Class, Transform);

// Deferred ：BeginPlay 前可设属性
AProjectile* P = World->SpawnActorDeferred<AProjectile>(Class, Transform);
P->Damage = 50.f;
P->FinishSpawning(Transform);   // 触发 BeginPlay

// 参数 ：Owner / Instigator / 碰撞处理
FActorSpawnParameters Params;
Params.Owner = this;
Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
```

**P34 - 三种查找：**
| API | 用途 |
| --- | --- |
| `FindComponentByClass<T>()` | 同类型只取第一个 |
| `GetComponents(T::StaticClass(), Out)` | 取全部 |
| `GetComponentsByTag(...)` / `ComponentHasTag` | 按 Tag 区分多个同类型 |

## 测试方法

**自动化测试：** `UETest.Fundamentals.P2_框架与资产 → A_Actor与Component`
```
P31 构造函数标记 Stage_Ctor
P32 RootScene 已创建 + 即 RootComponent
P34 三个 Mesh 默认带 Tag
P34 FindMeshByTag 正确返回
```

**手动 PIE：**
- P31 ：拖 Actor 到关卡 → 看 Output Log 的"Constructor → PostInit → BeginPlay → EndPlay → Destroyed"顺序
- P32 ：BP 调 `AddRuntimeMeshComponent` 看新组件出现
- P33 ：用 `SpawnDeferredBegin` + 设 Damage + `FinishDeferred` ；BeginPlay 时 Damage 已生效

## 常见坑
- 构造函数访问 `GetWorld()` → 崩；World 还没附着
- 运行时 `CreateDefaultSubobject` → 崩；只能在构造函数里
- 运行时 NewObject 出 Component 但忘了 RegisterComponent → 不 Tick / 不渲染
- SpawnActorDeferred 后忘了 FinishSpawning → BeginPlay 永远不触发，Actor 卡在初始化
