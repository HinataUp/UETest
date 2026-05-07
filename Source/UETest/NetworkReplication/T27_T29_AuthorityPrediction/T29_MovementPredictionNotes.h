// T29 - Movement Prediction 的观察（仅笔记，无新增运行时代码）
//
// CharacterMovementComponent 自带工业级移动预测 —— 已经在你 Possess 的角色上跑着。
// 这个 task 只用控制台命令模拟弱网，肉眼观察 CMC 的预测与纠正。
//
// 推荐操作步骤（PIE 控制台）：
//
//   net.PktLag 200            // 单向 200ms 延迟，手感能明显感受预测
//   net.PktLagVariance 50     // 抖动 50ms ，更接近真实公网
//   net.PktLoss 10            // 10% 丢包，更易触发纠正
//   p.NetShowCorrections 1    // 打开后，被 Server 纠正的瞬间会画一条红色线段
//   showdebug net             // 屏幕上叠加网络状态信息
//
// 观察要点：
//   1. 自机移动依然丝滑 —— Client 本地实时跑 Move ；
//   2. 撞到其他 Client 玩家时，可能瞬间被拉回 —— 纠正即"回到那一刻、按 Server 给的值，
//      用本地保存的输入历史重放余下帧"；
//   3. p.NetShowCorrections 红线起点是预测位置，终点是权威位置；线长越长偏差越大。
//
// 简历可写要点：
//   "理解 CMC 的 ClientPrediction / ServerCorrection 机制；能在弱网下定位走位拉扯并量化偏差。"
//
// 后续模块（GAS Prediction）会重用同样思想：
//   * 输入 -> 预测序号
//   * 本地立即生效
//   * Server 权威结果到达后比对/回滚
//
// 注：本头文件不参与构建逻辑，但放在源码树里方便查阅。
//     若不希望它被 UBT 收录，可改名 .md ；保留 .h 是为了与其他 task 文件统一目录结构。

#pragma once
