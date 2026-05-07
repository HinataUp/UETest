// 自动化测试 - 如何运行
//
// 本目录下所有 *Spec.cpp 都是 UE Spec 风格自动化测试。
// 它们用 #if WITH_DEV_AUTOMATION_TESTS 包裹，发布版本不会编译进来。
//
// ─────────────────────────── 一、UI 方式 ───────────────────────────
//   1) Editor 菜单：Window → Developer Tools → Session Frontend
//   2) 顶部切到 Automation 标签
//   3) 在搜索框里输入 "UETest.Network"
//   4) 勾选你想跑的分类（"属性复制 / RepNotify / RPC / 生命周期与组件"），
//      点 Start Tests
//   5) 测试结果按 Describe / It 树形展示，红绿图标一目了然
//
// 推荐第一次跑的勾选：UETest.Network ← 全选所有子节点
//
// ─────────────────────────── 二、命令行方式 ───────────────────────────
//   # Windows
//   "<UnrealEngine>\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" ^
//      "<Path>\UETest.uproject" ^
//      -ExecCmds="Automation RunTests UETest.Network; Quit" ^
//      -TestExit="Automation Test Queue Empty" ^
//      -ReportExportPath="<Path>\Saved\AutomationReports" ^
//      -unattended -nullrhi -log
//
//   # Linux / macOS
//   ./UnrealEditor-Cmd <Path>/UETest.uproject \
//      -ExecCmds="Automation RunTests UETest.Network; Quit" \
//      -unattended -nullrhi -log
//
//   * -nullrhi 表示无渲染，CI 跑得更快
//   * -unattended 关闭交互弹窗
//   * 最后会输出 JUnit / JSON 格式的报告，可接 CI（GitHub Actions / Jenkins）
//
// ─────────────────────────── 三、单独跑一个 Describe ───────────────────────────
//   Automation RunTests UETest.Network.RPC
//   Automation RunTests UETest.Network.属性复制
//
// 路径就是 BEGIN_DEFINE_SPEC 第二参数 + Describe 名字组合。
//
// ─────────────────────────── 四、单测能 / 不能覆盖 ───────────────────────────
//   ✓ 能覆盖：
//     * Replicated 字段是否注册到 GetLifetimeReplicatedProps
//     * UFUNCTION(Server/Client/NetMulticast, Reliable, WithValidation) 标记是否生效
//     * _Validate 业务逻辑（纯函数）
//     * _Implementation 业务逻辑（直接当成员函数调）
//     * USTRUCT 字段反射可读、UENUM 底层类型为 uint8
//     * Actor 默认配置：bReplicates / NetDormancy / bNetLoadOnClient / Component IsReplicated
//   ✗ 不能覆盖（必须手动 PIE）：
//     * OnRep 是否在远端真的触发
//     * Unmapped Reference 解析时序
//     * 同帧多次修改是否合并
//     * Multicast / Client RPC 是否真的到达远端
//     * NetDormancy 是否真的让 Server 跳过复制
//     * 客户端预测的纠正动画
//
// ─────────────────────────── 五、目录索引 ───────────────────────────
//   NetTestSpecUtils.h            公共辅助：反射查 RepProps / 函数标记
//   PropertyReplicationSpec.cpp   T1-T5 属性复制
//   RepNotifySpec.cpp             T6-T9 RepNotify 反射 + 业务直调
//   RPCSpec.cpp                   T16-T20 RPC 标记 + Validate 校验
//   LifecycleAndComponentSpec.cpp T14/T15/T21/T23/T24/T25/T26/T27/T28 综合

#pragma once
