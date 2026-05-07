# T30 调试工具速查

## 模块总览
没有工具的网络调试是盲人摸象。本目录把常用控制台命令收成一份速查表。本身不引入运行时代码，只是命令清单。

完成后你能：
- 在 PIE 控制台快速模拟弱网
- 看出 Replicated 字段在何时被复制、复制了多少
- 可视化移动纠正、查看复制带宽

## 使用方法
PIE 启动后按反引号 ` 键打开控制台，输入下方任一命令即可生效。命令仅在当前 PIE 会话有效，关掉 PIE 后失效。

打包到目标平台需要在 Project Settings → Network → Network Emulation Settings 配置（命令行参数也行）。

## 一、弱网模拟

| 命令 | 作用 | 推荐值 |
|---|---|---|
| `net.PktLag 200` | 单向延迟 200ms | 100-200 |
| `net.PktLagVariance 50` | 抖动 ±50ms | 20-50 |
| `net.PktLoss 20` | 20% 丢包 | 5-30 |
| `net.PktOrder 1` | 允许乱序到达 | 0/1 |
| `net.PktDup 5` | 5% 重复 | 罕用 |

**恢复**：每个参数设回 `0` 即可。

**典型弱网套餐：**
```
net.PktLag 100
net.PktLagVariance 20
net.PktLoss 5
```

## 二、查看 / 调试

| 命令 | 作用 |
|---|---|
| `stat Net` | 网络统计（每秒包数 / 字节数 / 连接数）|
| `stat NetPackets` | 包级别详情 |
| `net.Replication.Log 1` | 打印每个属性何时被复制（信息量大，用完关）|
| `showdebug net` | 屏幕叠加 NetMode / RTT / Replication 信息 |
| `showdebug netcorrection` | 显示移动纠正具体差值 |
| `p.NetShowCorrections 1` | 渲染纠正红线，**最直观** |
| `net.MaxRepArraySize` | 调高数组复制上限（罕用）|

## 三、GAS 相关（后续模块用）

```
showdebug abilitysystem
AbilitySystem.Debug.NextCategory
AbilitySystem.Debug.NextTarget
```

## 四、Listen Server 多窗口调试小贴士

- **关闭 Use Single Process**：Editor Preferences → Play → Advanced → 取消勾选；这样每个 PIE 窗口有独立 Output Log，日志不会混
- **Number of Players = 3 + Net Mode = Play As Listen Server**：第一个窗口既是 Server 也是 Player1，第二三窗口是纯 Client

## 五、个人手感调试套餐（推荐）

跑战斗时一键组合：
```
net.PktLag 100
net.PktLagVariance 20
net.PktLoss 5
p.NetShowCorrections 1
showdebug net
```

跑跑就能感受到："哦，原来 100ms 延迟是这种手感"——这是写网络代码的人必须建立的体感基线。

## 六、原理简述

**`net.PktLag` 在哪生效：**
- UE 在 NetDriver 层面延迟收发包，模拟传输层延迟
- 不是真改路由器配置，仅 PIE 进程内有效

**`p.NetShowCorrections` 怎么看：**
- 红色线段：起点 = Client 本地预测位置；终点 = Server 给的权威位置
- 线越长 = 预测偏差越大
- 频繁出现长线 = 网络抖动 + 本地预测算法弱

**`net.Replication.Log` 输出格式：**
```
[Net] LogProperty: 'Health' on 'BP_Character_C_0' replicated to client_1
[Net] LogProperty: 'Position' on 'BP_Character_C_0' replicated to client_1
...
```
- 能看到每个字段、每个 Connection、每次复制
- 极冗长，调试结束记得 `net.Replication.Log 0` 关掉

## 测试方法
- 这个 task 没有运行时代码，所以无自动化测试
- "测试方法"就是熟悉每条命令的输出 → 编写一份速查笔记 → 弱网下走一遍前面所有 task 验证你能解读屏幕信息

## 常见坑
- 命令拼错不会报错，只是无效；养成习惯回头看 `Cmd Output` 提示
- 弱网命令的状态在 PIE 间复用——下次启动可能仍带着 `net.PktLag=200`，如果手感诡异先 `net.PktLag 0` 重置
- `net.Replication.Log` 长开 → 性能直降，仅短期使用
- 真机包测试需要 Network Emulation Settings 而非控制台命令
