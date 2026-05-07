# E 章节 - GC / 断言 / 日志 (P25-P27)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P25 | GC 标记清除 + 根集 + 簇 | `P25_27_GCAssertLog` |
| P26 | check / ensure / verify | 同上 |
| P27 | 自定义 LogCategory + UE_LOG | 同上 |

## P25 - GC 机制

**核心规则：**
- GC 只管 UObject ；普通 C++ 对象不参与
- 根集 = 所有被 UPROPERTY / `AddToRoot` / `FGCObject` 引用的对象
- 标记-清除算法：从根集开始 Mark ，未被标记的对象在 Sweep 阶段销毁
- GC 簇（Cluster）优化：组内对象共生共死 ，减少 Mark 次数

**API：**
```cpp
GEngine->ForceGarbageCollection(/*bForceFullPurge=*/true);

UMyObject* Obj = NewObject<UMyObject>();
Obj->AddToRoot();           // 强制存活
Obj->RemoveFromRoot();      // 解除

bool bRooted = Obj->IsRooted();
```

**FGCObject 见 P16** ：非 UObject 类持有 UObject 引用的标准方案。

## P26 - 断言三件套

| 宏 | 失败行为 | Shipping |
| --- | --- | --- |
| `check(expr)` | 崩溃 | **编译掉**（expr 不执行） |
| `checkf(expr, fmt, ...)` | 崩溃 + 消息 | 同上 |
| `ensure(expr)` | 打 callstack 不崩 | 编译掉 |
| `ensureMsgf(expr, fmt, ...)` | 同上 + 消息 | 编译掉 |
| `verify(expr)` | 崩溃 | **保留 expr 副作用**（即使 Shipping 也执行）|

**经验：**
- 不变量 / 构造期 / "绝不可能为 nullptr" → `check`
- "不应发生但可继续"（不破坏游戏体验）→ `ensure`
- 表达式有副作用必须执行 → `verify`

```cpp
check(MeshComp != nullptr);                                 // 构造期
checkf(Health > 0, TEXT("Health=%f 非法"), Health);
ensure(IsInGameThread());                                   // 异步排查辅助
ensureMsgf(bReady, TEXT("系统未就绪"));
bool bOK = verify(LoadAsset());                             // Shipping 也加载
```

## P27 - LogCategory + UE_LOG

**自定义分类（推荐）：**
```cpp
// MyLog.h
DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);
// 第二参 ：默认级别（高于此级别才输出）
// 第三参 ：编译期最大级别（决定 Shipping 是否输出）

// MyLog.cpp
DEFINE_LOG_CATEGORY(LogMyGame);

// 使用
UE_LOG(LogMyGame, Warning, TEXT("【伤害】%s 受到 %.1f"), *Name, Dmg);
UE_LOGFMT(LogMyGame, Display, "玩家 {Name} 等级 {Level}", Name, Level);  // UE5.2+
```

**五级日志：** Verbose < Log < Display < Warning < Error。
**控制台过滤：** `log LogMyGame Verbose` / `log LogMyGame Off`。

## 测试方法

**自动化测试：** 暂未为 GC / 断言 / 日志单独写 Spec ；这部分行为更适合在 `Tests/Part1Spec.cpp` 的其他测试间接验证（如 P02 CDO 测试用了反射 + Spec 输出）。

**手动 PIE：**
- P25 ：调 `UP25_27_GCAssertLog::ForceFullGC` ；用 `MemReport -full` 看 UObject 数变化
- P26 ：调 `DemoEnsure(-1)` → Output Log 看 callstack ，进程不崩
- P27 ：调 `DemoLog` ；控制台 `log LogP1_GCAssert Verbose` 切换级别

## 常见坑
- 把 UObject 存到普通 `TArray<UMyObj*>` 而不是 `TArray<TObjectPtr<UMyObj>>` 的 UPROPERTY → GC 不知道引用 ，被回收
- ensure 在 Shipping 等于直接 if(false) 跳过 → 不能用作"必须保留的副作用"
- 关闭 LogCategory（log MyCat Off）后忘了恢复 → 调试时一片黑
- 频繁 ForceGarbageCollection → 卡顿 ；调试期才用
