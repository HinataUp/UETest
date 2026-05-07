# GAS 自动化测试

## 测试取舍
和网络同步模块同样的策略：

| 层级 | 适合测什么 | 不适合 |
| --- | --- | --- |
| **Spec 单测** | 配置正确性 / Tag 注册 / GE 类绑定 / 反射检查 | 时序、Periodic、网络复制 |
| **手动 PIE** | OnRep 跨端、Cue 同步、预测回滚动画 | 重复回归 |

## 文件
| 文件 | 覆盖 |
| --- | --- |
| `GASTestSpecUtils.h` | `FTestEnv::Setup()` 一行起 ASC + HealthSet + CombatSet |
| `A_BasicsSpec.cpp` | A01-A05：双值结构、多 Set、初始化、Tag 层级、Clamp |
| `B_GameplayEffectSpec.cpp` | B06/07/08/10/11/14：Instant/Duration/Infinite/Modifier 顺序/SetByCaller/Immunity |
| `CD_AbilityAndDamageSpec.cpp` | C/D：GA 默认配置、Cost/CD 绑定、GA Tag 五件套、Meta Attribute 管线 |
| `GHI_TagNetworkSpec.cpp` | G/H/I：Loose vs GE Tag、GE 授技能、PredictionRollback 配置 |

## 输出格式
```
UETest.GAS.A基础设施
  A01 AttributeSet 基础
    无 Modifier 时 Base 应等于 Current        ✓
  A02 多 AttributeSet 共存
    HealthSet 与 CombatSet 都已注册            ✓
  A03 GE 初始化 vs Native InitXxx
    Native InitHealth 不触发 PostExecute       ✓
    GE_InitPlayerStats 走完整管线，触发 PostExecute  ✓
  ...
```

## 运行方式

**UI：** Editor → Window → Developer Tools → Session Frontend → Automation → 搜 `UETest.GAS` → Start Tests

**命令行：**
```bash
UnrealEditor-Cmd.exe UETest.uproject \
  -ExecCmds="Automation RunTests UETest.GAS; Quit" \
  -unattended -nullrhi -log
```

**单跑某章节：**
```
Automation RunTests UETest.GAS.A基础设施
Automation RunTests UETest.GAS.B_GameplayEffect
```

## 单测覆盖度

✓ **能覆盖：**
- AttributeSet 字段已注册到复制系统
- GE 类构造期 Modifier / Tag / Component 配置正确
- GA 默认 InstancingPolicy / NetExecutionPolicy
- _Validate 业务逻辑、_Implementation 业务逻辑
- Tag 层级匹配、Container 操作
- Loose Tag / GE-Driven Tag 增删行为

✗ **不能覆盖（必须手动 PIE）：**
- OnRep 跨端时机
- Periodic GE 的 Tick 时序
- Duration GE 的到期自动移除
- Cue 网络广播
- LocalPredicted 的预测回滚视觉
- Movement Prediction 纠正

## 如何扩展

为新 GE 增加测试 ：仿照 `B_GameplayEffectSpec.cpp` 的模板：
```cpp
It(TEXT("MyNewGE Apply 后效果正确"), [this]()
{
    FTestEnv Env;
    Env.Setup();
    if (!Env.IsValid()) return;

    Env.HealthSet->InitHealth(100.f);
    ApplyGEToSelf(Env.ASC, UMyNewGE::StaticClass());
    TestEqual(TEXT("【My GE】预期效果"),
        Env.ASC->GetNumericAttribute(UHealthSet::GetHealthAttribute()), 80.f);
});
```
