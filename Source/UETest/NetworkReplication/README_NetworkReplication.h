// 网络同步模块 - 代码示例索引
// 用 .h 后缀仅为放在源码树里方便查阅；不参与运行时逻辑。
//
// 目录结构（每个文件夹一类，每类内多个 task 各自一对 .h/.cpp）：
//
//   Common/                          公共日志宏 NET_LOG / NET_SCREEN_LOG
//   T0_BasicSetup/                   基础测试 Actor（携带 StaticMesh）
//   T1_T5_PropertyReplication/       属性复制基础
//       T1  bReplicates 与 Replicated 标记
//       T2  基础类型（int/float/bool/FString/FName）
//       T3  USTRUCT
//       T4  TArray（含结构体数组）
//       T5  UENUM / FName 低成本同步
//   T6_T9_RepNotify/                 RepNotify 变体
//       T6  无参 OnRep
//       T7  带 OldValue 的 OnRep
//       T8  REPNOTIFY_Always 与"让 Authority 也走 OnRep"
//       T9  同帧多次修改的合并行为
//   T10_T13_PointerReplication/      指针复制
//       T10 关卡 Actor 引用
//       T11 运行时 Spawn Actor 引用（Unmapped）
//       T12 普通 UObject 失败示例
//       T13 Component 引用
//   T14_T15_ConditionalReplication/  条件复制
//       T14 五种 DOREPLIFETIME_CONDITION
//       T15 PreReplication 动态条件
//   T16_T20_RPC/                     RPC 变体
//       T16 Server RPC（Reliable / Unreliable）
//       T17 Client RPC
//       T18 NetMulticast RPC
//       T19 RPC 参数类型综合
//       T20 _Validate 校验
//   T21_T24_ActorLifecycle/          Actor 生命周期与所有权
//       T21 Spawn / Destroy 时序
//       T22 SetOwner 与 Autonomous
//       T23 NetDormancy
//       T24 bNetLoadOnClient
//   T25_T26_ComponentReplication/    Component 复制
//       T25 Replicated Component 基础
//       T26 ReplicatedSubObject（GAS 同款机制）
//   T27_T29_AuthorityPrediction/     权威与预测
//       T27 Server 权威最小闭环
//       T28 客户端预测最简形态
//       T29 Movement Prediction 观察笔记
//   T30_DebugTools/                  网络调试命令速查
//
// 测试运行方式：
//   * 在 Editor 任意关卡放置对应 Actor / Pawn ；
//   * Editor → Play → Number of Players = 3 ，Net Mode = Play As Listen Server ；
//   * 三个独立窗口的 Output Log 中观察 NET_LOG 输出。

#pragma once
