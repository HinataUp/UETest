# T27-T29 权威与预测

## 模块总览
前面的 task 都在讲机制；本目录把"输入 → Server → 复制回 Client"串成完整闭环，并在此之上加客户端预测。这是 **GAS Prediction / 多人战斗** 的认知地基。

| Task | 主题 | 文件 |
|---|---|---|
| T27 | Server 权威最小闭环（无预测）| `AT27_ServerAuthorityCharacter` |
| T28 | 客户端预测最简形态 | `AT28_ClientPredictionCharacter` |
| T29 | Movement Prediction 观察笔记 | `T29_MovementPredictionNotes.h` |

完成后你能回答：
- 什么是"权威模式"？为什么手感差？
- 客户端预测的基本思想是什么？
- CMC 的纠正是怎么发生的？
- 哪些值适合预测，哪些不适合？

## 使用方法

**T27 / T28：**
1. 设置 GameMode 的 Default Pawn Class 为 `AT27_ServerAuthorityCharacter` 或 `AT28_ClientPredictionCharacter`
2. 三窗口 PIE
3. 各 Client 按 **F 键** → 触发 ServerRequestTakeDamage

**T29（仅观察，无新代码）：**
1. 用默认 ThirdPerson 角色或 T28 跑起来
2. PIE 控制台 `（` 键打开）输入：
   ```
   net.PktLag 200
   net.PktLagVariance 50
   net.PktLoss 10
   p.NetShowCorrections 1
   ```
3. 移动并撞向其他 Client → 观察被纠正时的红色线段

## 实现要点

**T27 权威模式三步：**
```cpp
// Step 1: Client 输入触发 Server RPC
void AT27Character::OnPressF()
{
    ServerRequestTakeDamage(10);   // 不本地修改！
}

// Step 2: Server 校验 + 执行
bool AT27Character::ServerRequestTakeDamage_Validate(int32 Amount) { return Amount > 0 && Amount <= 1000; }
void AT27Character::ServerRequestTakeDamage_Implementation(int32 Amount)
{
    Health = FMath::Max(0, Health - Amount);
}

// Step 3: 复制 Health 回 Client + OnRep 表现
UPROPERTY(ReplicatedUsing = OnRep_Health) int32 Health;
```

**T28 加预测：**
```cpp
void AT28Character::OnPressF()
{
    // 1) 本地"乐观预测"：立刻扣血，UI 即刻反馈
    if (GetLocalRole() == ROLE_AutonomousProxy)
    {
        Health = FMath::Max(0, Health - 10);
    }

    // 2) 发 RPC 让 Server 处理
    ServerRequestTakeDamage(10);
}

// Server 处理时若拒绝（如 Health<=50）：什么都不改
// → Client 收到的是"权威值还是原来"
// → OnRep 用权威值覆盖 Client 的预测
```

## 原理简述

**为什么权威模式手感差：**
- 按键 → Server → 复制回来 = 1 个 RTT 的延迟
- 60ms 网络下 → 至少 60ms 才能看到反馈
- 在快节奏战斗里这个延迟手感很糟糕

**预测的核心思想：**
> Client 假装已经发生 → Server 稍后确认 → 不一致就纠正

**预测的代价：**
- 不一致时要"回滚"：本地撤销假设的改动，应用 Server 给的真值
- 实现复杂度高：要保留输入历史、回放余下帧
- 玩家可能短暂看到"果不其然"或"莫名被拉回"的视觉效果

**适合 / 不适合预测：**
| 适合 | 不适合 |
|---|---|
| 移动 / 输入响应（手感敏感）| 全局状态 / 关键判定 |
| 自机姿态 / 动画 | 伤害数值 / 命中结果 |
| 技能开始释放（"我出招了"）| 命中确认（"打中了多少"）|

**CMC（CharacterMovementComponent）的预测：**
- 输入 → 本地立即跑 Move
- 同时记录"输入 + 时间戳"到历史队列
- Server 周期性回发权威位置
- Client 比对：差太多 → "回滚到那一帧 + 重放历史输入"
- `p.NetShowCorrections` 红线 = 预测点 → 权威点 的偏差可视化

**GAS Prediction 是同样思想的工业级实现：**
- 用 `FPredictionKey` 标记每次预测
- `LocalPredicted` 模式：Client 立即应用 GameplayEffect
- Server 确认 → 提交 / 回滚

## 测试方法

**手动 PIE：**
- T27：按 F 后能感觉到延迟（PIE 本地几乎无感，加 `net.PktLag 200` 能明显感受）
- T28：按 F 后本地立即扣血；等 Server 回复（OnRep）确认
- T28 拒绝场景：在 Health=51 时按 F → 本地预测扣到 41 → Server 拒绝 → OnRep 把 Health 拉回 51（注意：本示例为简化版，没做"强制再同步"机制，回滚行为依赖 OnRep 触发）
- T29：观察红色纠正线

**自动化测试：** `UETest.Network.生命周期与组件 → T27 / T28`

```
覆盖：
  T27 Server 权威：业务逻辑直调
    - 正常请求扣 10 血，Health 应从 100 -> 90
    - Validate 拒绝越界 Damage
  T28 客户端预测：业务逻辑直调
    - Health <= 50 时 Server 拒绝扣血（值不变）
```

注：**预测的视觉纠正、CMC 的 NetShowCorrections** 只能 PIE 观察。

## 常见坑
- T28 的简化版没实现"权威值与本地预测相等时的强制同步" → 实际工程要用 `REPNOTIFY_Always` + 预测序号机制
- 在 Server 端跑客户端预测代码 → Server 既是 Authority 又有 Local 角色，注意 `GetLocalRole()` 判断
- 预测过头："连命中都本地预测" → 出现"我看到打中、但没掉血"的诡异同步
- 不预测：手感长期差 → 玩家流失（特别是 PVP）
