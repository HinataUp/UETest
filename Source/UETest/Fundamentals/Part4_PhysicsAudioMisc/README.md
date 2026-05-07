# UE5 基础任务清单 · Part 4 ：物理 / 音频 / 特效 / 工程补充

## 总览
15 个任务覆盖 ：碰撞 / Trace / Physics ；Niagara VFX ；MetaSounds / SoundCue 音频 ；Sequencer ；PCG ；Materials from C++ ；RenderTarget ；Editor Tools ；打包验证。

## 章节地图

| 章节 | 任务 | 主题 | 文件夹 |
| --- | --- | --- | --- |
| **A** | P86-P88 | 物理与碰撞 | `A_Physics/` |
| **B** | P89-P90 | Niagara VFX | `B_Niagara/` |
| **C** | P91-P92 | 音频系统 | `C_Audio/` |
| **D** | P93-P96 | Sequencer + PCG + Materials + RT | `D_Sequencer/` |
| **E** | P97-P98 | Editor Tools + 打包 | `E_EditorPackaging/` |
| 公共 | — | 日志宏 | `00_Common/` |
| 测试 | — | Spec | `Tests/` |

## 任务一览

### A - 物理（3）
- P86 - Collision Channel + Response 矩阵
- P87 - Line / Sweep / Overlap Trace
- P88 - Physics Simulation + Force / Impulse

### B - Niagara（2）
- P89 - C++ 生成 + 参数控制
- P90 - Data Interface（Mesh / Array / RenderTarget）

### C - 音频（2）
- P91 - MetaSounds（UE5 推荐）
- P92 - SoundCue + Attenuation + Concurrency

### D - Sequencer + 材质（4）
- P93 - LevelSequence C++ 播放
- P94 - PCG 程序化生成
- P95 - MID + MPC（与 Rendering T11/T04 概念一致 ；本 Helper 简化版）
- P96 - Render Target + Canvas（与 Rendering T12 简化版）

### E - 工程补充（2）
- P97 - Editor Utility Widget + 批量操作
- P98 - Packaging / Cooking / Build Configuration

## 工程化

**Build.cs 添加：** `Niagara / LevelSequence / MovieScene`

**插件按需启用：**
- PCG（P94）：Procedural Content Generation Framework
- Editor Scripting Utilities + Blutility（P97）

## 自动化测试

`UETest.Fundamentals.P4_物理音频特效`
```
A_物理碰撞      ：3 用例
B_Niagara       ：1 用例
C_音频           ：1 用例
D_Sequencer与材质：3 用例
E_编辑器与打包   ：3 用例
```

详见 `Tests/README.md`。

## 与现有模块的边界

| 知识点 | 详尽位置 | Part 4 处理 |
| --- | --- | --- |
| Material / RT / Shader | `Source/UETest/Rendering/` | P95 / P96 给 BP 友好 Helper（与 T11 / T12 互为表里）|
| 网络复制 | `Source/UETest/NetworkReplication/` | 不重复 |
| GAS | `Source/UETest/GAS/` | 不重复 |
| C++ 核心 / Framework | `Fundamentals/Part1 / Part2` | 复用 |
| UI / AI / 动画 | `Fundamentals/Part3` | 不重复 |

## 学习路径建议

- **物理碰撞（P86-P88）** 最容易上手 ：先把 Trace 用熟
- **Niagara（P89-P90）** ：实际项目几乎必用 ；先掌握 SpawnSystemAtLocation + User Parameter
- **音频（P91-P92）** ：MetaSounds 是新项目首选 ；老项目维护 SoundCue
- **Sequencer（P93）** ：过场动画 / 镜头控制 ；P94 PCG 大世界生态
- **MID / MPC / RT（P95-P96）** ：视觉表现的代码桥梁 ；与 Rendering 模块互为参考
- **Editor / Packaging（P97-P98）** ：项目工程化收尾 ；Shipping 前的"最后一公里"

## 至此 Fundamentals 系列四 Part 完整覆盖

| Part | 任务 | 主题 |
| --- | --- | --- |
| Part 1 | P01-P30 | C++ 核心 / 对象模型 |
| Part 2 | P31-P60 | Gameplay 框架 / 资产 / 子系统 |
| Part 3 | P61-P85 | UI / AI / 动画 |
| Part 4 | P86-P98 | 物理 / 音频 / 特效 / 工程 |

加上 ：
- 网络同步（T0-T30，30 task）
- GAS（A01-J49，47 task）
- 渲染 / Shader（T01-T16，15 task）

整个仓库共计 **~190 个 task** 的 UE5.6 工业级 C++ 示例 + 自动化测试 + 中文 README。
