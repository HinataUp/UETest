# UE5 基础任务清单 · Part 3 ：UI + AI + 动画系统

## 总览
25 个任务覆盖 ：UMG / Slate / Common UI / HUD ；BehaviorTree / EQS / AI Perception / NavMesh / StateTree / SmartObjects / Mass Entity ；AnimInstance / State Machine / BlendSpace / Montage / Notify / IK / Linked Anim Graph / Control Rig / Motion Matching。

## 章节地图

| 章节 | 任务 | 主题 | 文件夹 |
| --- | --- | --- | --- |
| **A** | P61-P69 | UMG / Slate / UI | `A_UI/` |
| **B** | P70-P77 | AI 系统 | `B_AI/` |
| **C** | P78-P85 | 动画系统 | `C_Animation/` |
| 公共 | — | 日志宏 | `00_Common/` |
| 测试 | — | Spec | `Tests/` |

## 任务一览

### A - UI（9）
- P61 - UUserWidget + BindWidget
- P62 - 事件绑定 + 数据驱动
- P63 - UListView 数据驱动列表
- P64 - Common UI Plugin（README）
- P65 - Slate SCompoundWidget
- P66 - Slate 布局（README）
- P67 - FSlateStyleSet 自定义视觉风格（README）
- P68 - AHUD + Canvas 绘制
- P69 - Input Mode 管理

### B - AI（8）
- P70 - BehaviorTree Task / Decorator / Service
- P71 - EQS Environment Query System（README）
- P72 - AI Perception（视觉 / 听觉 / 伤害）
- P73 - NavMesh + PathFollowing
- P74 - StateTree（README）
- P75 - Smart Objects（README）
- P76 - Mass Entity（ECS）（README）
- P77 - Mass Spawner + ISM Crowd（README）

### C - 动画（8）
- P78 - AnimInstance C++ 驱动 AnimBP
- P79 - State Machine + BlendSpace（README）
- P80 - AnimNotify + AnimNotifyState
- P81 - IK 系统（FABRIK / TwoBone）（README）
- P82 - Linked Anim Graph 分层动画（README）
- P83 - Control Rig（C++ Helper）
- P84 - Motion Matching / PoseSearch（README）
- P85 - Distance Matching / Stride Warping（README）

## 工程化

**Build.cs 已添加：**
```csharp
"UMG", "Slate", "SlateCore",      // UI
"AIModule", "NavigationSystem"    // AI / Navigation
```

**插件可选启用（README 描述对应 task）：**
- Common UI（P64）
- StateTree（P74）
- SmartObjects（P75）
- MassEntity / MassGameplay（P76 / P77）
- PoseSearch（P84）
- ControlRig（P83 完整模式）

## 自动化测试

`UETest.Fundamentals.P3_UI_AI_动画`
```
A_UI    ：4 用例（BindWidget / 委托 / ListEntry / HUD 默认）
B_AI    ：4 用例（BTTask / BTDecorator / Perception / NavMeshHelper）
C_动画  ：3 用例（AnimInstance 字段 / Notify 子类 / ControlRig Helper）
```

详见 `Tests/README.md`。

## 学习路径建议

**周 1（UI）：**
1. P61 + P62 ：跑通最基础的 BindWidget + 事件
2. P63 ListView ：理解虚拟化列表
3. P68 + P69 ：HUD + Input Mode
4. 若需要 AAA 风格菜单 → P64 Common UI ；底层基础 → P65/66/67 Slate

**周 2（AI）：**
1. P70 + P73 ：搭最小 AI 闭环（BT 巡逻 + NavMesh 寻路）
2. P72 ：加感知
3. P71 EQS ：精细化目标选择
4. UE5 新 ：P74 StateTree（可选，团队接受度看情况）
5. 性能 ：P76/P77 Mass Entity（千级 NPC 时考虑）

**周 3（动画）：**
1. P78 + P79 ：AnimInstance + BlendSpace（基础 ：3D 角色移动动画）
2. P80 ：Montage + Notify（连招 / 命中帧）
3. P81 ：IK（脚部地面 / 武器握把）
4. P82 ：分层（跑步 + 上半身攻击）
5. UE5 新 ：P83/P84/P85（程序化 / 自动匹配）

## 与其他模块的边界

| 知识点 | 详尽位置 | Part 3 处理 |
| --- | --- | --- |
| GAS Ability | `GAS/` | 不重复 ；P80 通过 GameplayEvent 与 GAS 集成 |
| Renderer / Material | `Rendering/` | 不重复 |
| C++ 核心 | `Fundamentals/Part1_CppCore/` | 复用反射 / 委托模式 |
| Framework / Asset | `Fundamentals/Part2_FrameworkAssets/` | 复用 PlayerController / 输入 |

## 现有模块整体关系

```
Part 1 (P01-P30) ：C++ 核心 / 对象模型
Part 2 (P31-P60) ：Gameplay 框架 / 资产 / 子系统
Part 3 (P61-P85) ：UI / AI / 动画           ← 本目录
NetworkReplication (T0-T30) ：网络同步 30 task
GAS (A01-J49) ：GAS 47 task
Rendering (T01-T16) ：渲染 / Shader 15 task
```
