# A 章节 - GAS 基础设施 (A01-A05)

## 模块总览
GAS 的"四件套"基础：AttributeSet / Replicated 属性 / GameplayTag / Hook。掌握后才能进入 B 章节的 GE 系统。

| Task | 主题 | 文件 |
| --- | --- | --- |
| A01 | AttributeSet 双值结构 + Replicated | `A01_HealthSet.h/cpp` |
| A02 | 多 AttributeSet 共存 + 拆分原则 | `A02_CombatSet.h/cpp` |
| A03 | GE 初始化 vs Native InitXxx | `A03_GE_InitPlayerStats.h/cpp` |
| A04 | Native GameplayTag 注册 | `../00_Common/MyGameplayTags.h/cpp` |
| A05 | PreAttributeChange / PostExecute Hook | 在 `A01_HealthSet` 中实现 |

完成后能回答：
- BaseValue 与 CurrentValue 的差别？
- 为什么 OnRep 必须调 `GAMEPLAYATTRIBUTE_REPNOTIFY`？
- 一个 AttributeSet 装多少属性合适？
- Native vs GE 初始化的取舍？
- 死亡判定该放 Pre 还是 Post？

## 使用方法

**手动 PIE：**
1. GameMode 的 PlayerStateClass = `AGASTestPlayerState`
2. PIE 启动 → 三端日志能看到 HealthSet 初始化（默认 100/100）
3. 控制台 `showdebug abilitysystem` 查看属性当前值

**自动化测试：** `UETest.GAS.A基础设施`

## 实现要点

**ATTRIBUTE_ACCESSORS 宏一次生成 4 个函数：**
```cpp
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \  // GetXxxAttribute() 拿 FGameplayAttribute
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \                // GetXxx() 拿 float
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \                // SetXxx(float) 改
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)                 // InitXxx(float) 不走 Hook
```

**OnRep 标准模式：**
```cpp
UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
FGameplayAttributeData Health;

UFUNCTION()
void OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, Health, OldHealth);
    // 这一行是关键 ：缺了 Client 端聚合器不刷新
}
```

**Pre vs Post 分工：**
| Hook | 触发场景 | 职责 |
| --- | --- | --- |
| PreAttributeChange | 所有路径（GE/Set/Modifier 聚合）| Clamp 数值 |
| PostGameplayEffectExecute | 仅 Instant GE 改 BaseValue 后 | 反应逻辑（死亡、事件、Meta 转换）|

## 原理简述

**双值结构：**
- BaseValue ：永久存储，只能由 Instant/Periodic GE 或直接 Set 修改
- CurrentValue = Base + ΣAdd × ΠMul（受 Duration/Infinite GE 的 Modifier 聚合影响）
- 移除 Buff 时 ：Modifier 撤销 → CurrentValue 重新计算 → 数值回落

**为什么不直接复制 float：**
- 若直接 `UPROPERTY(Replicated) float Health` → ASC 内部聚合器不知道值变了 → 旁路逻辑错乱
- 走 `FGameplayAttributeData` + GAS 标准 Hook → 聚合器、Cue、Delegate 全联动

**多 Set 拆分原则：**
- 同一 UClass 不能注册两个实例（第二个静默被忽略）
- 每个 Set 5-15 个属性，按关注点分（生命/战斗/资源）
- 拆太细 → Cross-Set 依赖增多，MMC/ExecCalc 捕获参数变多

## 测试方法

**自动化测试覆盖：**
```
UETest.GAS.A基础设施
  A01 AttributeSet 基础
    无 Modifier 时 Base 应等于 Current
  A02 多 AttributeSet 共存
    HealthSet 与 CombatSet 都已注册
  A03 GE 初始化 vs Native InitXxx
    Native InitHealth 不触发 PostExecute
    GE_InitPlayerStats 走完整管线，触发 PostExecute
  A04 GameplayTag 层级匹配
    子 Tag MatchesTag 父 Tag 应为 true
    Container HasAny 匹配子树
  A05 PreAttributeChange Clamp
    超 MaxHealth 的设值应被 Clamp
```

**手动 PIE 验收：**
- 给玩家 Apply `UGE_InitPlayerStats` ：三端 HealthSet/CombatSet 初值同步到 100/20
- Server 端 `SetNumericAttributeBase(Health, 200)` ：本地 Clamp 到 100，三端日志显示 Pre 触发但 Post 不触发

## 常见坑
- 漏写 `GAMEPLAYATTRIBUTE_REPNOTIFY` → Client 端属性看似变了但聚合器没刷新
- 死亡判定写在 Pre → 还没 Clamp 完就触发，时序错乱
- 用 Native InitXxx 初始化但期望 PostExecute 跑 → Native 不走完整管线
- 同一类 AttributeSet 注册两次（写两个 `CreateDefaultSubobject<UHealthSet>`）→ 第二个被静默忽略
