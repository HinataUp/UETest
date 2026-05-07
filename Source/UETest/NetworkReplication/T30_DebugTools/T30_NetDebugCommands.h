// T30 - 网络调试命令速查
//
// 本头文件不引入运行时代码，仅作为速查笔记。
// 所有命令在 PIE 控制台（` 键打开）输入即可生效。打包到目标平台需要 Network Emulation Settings 。
//
// 一、弱网模拟（用来重现真实玩家的体验）
//   net.PktLag 200            // 单向延迟 200ms
//   net.PktLagVariance 50     // 抖动 ±50ms
//   net.PktLoss 20            // 20% 丢包
//   net.PktOrder 1            // 允许乱序
//   net.PktDup 5              // 5% 重复（罕见用）
//
//   恢复：把上面参数都设回 0 即可。
//
// 二、查看 / 调试
//   stat Net                  // 网络统计：每秒包数、字节数、连接数
//   stat NetPackets           // 包级别详情
//   net.Replication.Log 1     // 输出每个属性何时被复制；信息海量，短期开启
//   showdebug net             // 屏幕上叠加 NetMode / RTT / Replication 信息
//   showdebug netcorrection   // 显示移动纠正的具体差值
//   p.NetShowCorrections 1    // 渲染纠正红线，最直观
//   net.MaxRepArraySize       // 调高数组复制上限（默认有保护值）
//
// 三、GAS 相关（后续模块用）
//   showdebug abilitysystem
//   AbilitySystem.Debug.NextCategory
//   AbilitySystem.Debug.NextTarget
//
// 四、Listen Server 多窗口调试小贴士
//   * Editor → Editor Preferences → Play → Use Single Process: 关闭
//     这样每个 PIE 窗口有独立 Output Log ，日志不混；
//   * Number of Players = 3 + Net Mode = Play As Listen Server
//     => Server / Client1 / Client2 三窗对照最直观。
//
// 五、个人手感调试套餐（推荐）
//   net.PktLag 100
//   net.PktLagVariance 20
//   net.PktLoss 5
//   p.NetShowCorrections 1

#pragma once
