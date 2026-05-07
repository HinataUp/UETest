# GAS 学习模块（UE5.6）

## 总览
UE5.6 GameplayAbilitySystem 的 47 个核心 task，按 10 个章节组织，覆盖从基础设施到工业级战斗系统。

每个章节一个文件夹 + 一份 README.md，包含：模块总览、使用方法、实现要点、原理简述、测试方法、常见坑。

## 章节地图

| 章节 | 任务 | 主题 | 文件夹 |
| --- | --- | --- | --- |
| **A** | A01-A05 | 基础设施：AttributeSet / Tag / Hook | `A_Basics/` |
| **B** | B06-B14 | GameplayEffect 完整覆盖 | `B_GameplayEffect/` |
| **C** | C15-C21 | GameplayAbility 完整覆盖 | `C_GameplayAbility/` |
| **D** | D22-D27 | 伤害管线工业化 | `D_DamagePipeline/` |
| **E** | E28-E32 | AbilityTask | `E_AbilityTask/` |
| **F** | F33-F34 | GameplayCue | `F_GameplayCue/` |
| **G** | G35-G37 | Tag 系统进阶 | `G_TagSystem/` |
| **H** | H38-H42 | 实战系统集成 | `H_Integration/` |
| **I** | I43-I46 | GAS 网络（Mixed/Minimal/Predict）| `I_Network/` |
| **J** | J47-J49 | 调试工具与自查清单 | `J_DebugTools/` |
| 公共 | — | 日志 / Tag / PlayerState / 测试 Actor | `00_Common/` |
| 测试 | — | 各章节自动化 Spec | `Tests/` |

## 快速上手

### 1. 配置项目
- `UETest.uproject` 已启用 GameplayAbilities 插件
- `UETest.Build.cs` 已添加 `GameplayAbilities / GameplayTags / GameplayTasks` 依赖
- `00_Common/MyGameplayTags.cpp` 用 Native 注册了所有项目 Tag

### 2. 设置 GameMode
```cpp
// 在 GameMode 类里
PlayerStateClass = AGASTestPlayerState::StaticClass();
DefaultPawnClass = ACharacter::StaticClass();
```

### 3. 三端验证
- Editor → Play → Number of Players = 3 → Net Mode = Play As Listen Server
- 控制台 `showdebug abilitysystem` 看 ASC 状态

### 4. 运行测试
- Window → Developer Tools → Session Frontend → Automation
- 搜 `UETest.GAS` → Start Tests
- 期待全绿（详见 `Tests/README.md`）

## 模块约定

**命名前缀：**
- `UHealthSet / UCombatSet` ：AttributeSet
- `UGE_xxx` ：GameplayEffect 子类
- `UGA_xxx` ：GameplayAbility 子类
- `UMMC_xxx` / `UExecCalc_xxx` ：MMC / ExecCalc
- `UCueNotify_xxx` / `ACueNotify_xxx` ：Static / Actor 型 Cue
- 公共基础放 `00_Common/`

**日志：** `GAS_LOG("【中文标签】英文变量+数字")`，分类 `LogGASTest`，Warning 级。

**复制策略：** Player ASC 在 PlayerState（Mixed），AI ASC 在 Character（Minimal）。

**Tag 命名：**
```
Ability.Skill.<技能名>          技能身份
State.<状态>                    角色状态
Effect.<分类>.<子类>            GE 资产标签
Cooldown.Skill.<技能名>         冷却 Tag
GameplayCue.<分类>.<子类>       Cue Tag
SetByCaller.<参数名>            SetByCaller 参数 Tag
Input.<输入>                    Input 绑定 Tag
Event.<事件>                    GameplayEvent Tag
```

## 学习路径建议

**新人路径（先跑通基本盘）：**
1. A 章节 ：理解 AttributeSet 双值结构
2. B06-B08 ：跑过 Instant / Duration / Infinite 三种 GE
3. C15 + C19 ：写出第一个能 Cost+CD 的 GA
4. F33 ：加点视觉表现
5. 此时已能搭出最小可玩战斗

**进阶（工业化）：**
- D 全章 ：伤害公式工业化（Meta + ExecCalc + Crit Context）
- H40 + H41 ：输入缓冲 + 连招（ACT 手感核心）
- I 全章 ：搞懂网络与预测
- J 全章 ：装备调试武器库

## 已剔除的反模式

按 v2 任务文档的明确指引，本模块**不演示**以下"看起来能用但坑深"的写法：

- `Full` ReplicationMode（多人项目带宽爆炸）
- 频繁 Add/Remove AttributeSet（官方明确警告）
- NonInstanced GA + AbilityTask 组合（CDO 共享导致状态污染）
- 已废弃的 `GrantedApplicationImmunityTags` 字段（用 `UImmunityGameplayEffectComponent` 替代）

## 后续扩展建议

- 接入实际 Skeletal Mesh + Montage 资产（E29 真正播动画）
- 把 D24 ExecCalc 的暴击数字接到 Cue 端做飘字（D26 + F33 综合）
- 实现 `DefaultGame.ini` 注册 `MyAbilitySystemGlobals`（让自定义 EffectContext 真正全局生效）

## 测试与验证

- 每章节 README 末尾有 "测试方法" 段，明确写出哪些用 PIE、哪些有 Spec 测试
- `Tests/README.md` 给出整体运行方式
- 详细问题请查 `J_DebugTools/README.md` 的"常见 Bug 自查清单"
