# F 章节 - 异步 / 线程 / 定时器 (P28-P30)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P28 | FTimerManager | `P28_30_AsyncShowcase` |
| P29 | UE::Tasks + Async + ParallelFor | 同上（演示 Async）|
| P30 | GameThread 调度 + FCriticalSection | 同上 |

## P28 - FTimerManager

```cpp
FTimerHandle Handle;
GetWorldTimerManager().SetTimer(
    Handle, this, &AMyActor::OnTimer,
    /*Time=*/2.f, /*bLoop=*/false);

// Lambda 版本
GetWorldTimerManager().SetTimer(
    Handle,
    FTimerDelegate::CreateLambda([]() { /* ... */ }),
    1.f, false);

// 下一帧执行
GetWorldTimerManager().SetTimerForNextTick(
    FTimerDelegate::CreateUObject(this, &AMyActor::OnNext));

GetWorldTimerManager().ClearTimer(Handle);
bool bActive = GetWorldTimerManager().IsTimerActive(Handle);
```

**关键认知：** Timer 在 GameThread 上推进 ；暂停 Game（PIE 暂停）时 Timer 也暂停。Owner Actor 销毁时 Timer 自动清理。

## P29 - 异步 API

```cpp
// UE::Tasks（UE5 推荐）
UE::Tasks::FTask Task = UE::Tasks::Launch(TEXT("Heavy"), []() {
    // 后台线程
    DoExpensiveWork();
});
Task.Wait();   // 阻塞等待

// Async（兼容老 API）
TFuture<int32> F = Async(EAsyncExecution::ThreadPool, []() -> int32 {
    return ExpensiveCompute();
});
int32 V = F.Get();   // 阻塞获取

// ParallelFor
ParallelFor(NumItems, [&](int32 Index) {
    Results[Index] = ComputeItem(Index);
});
```

**EAsyncExecution 选项：**
| 值 | 含义 |
| --- | --- |
| `Thread` | 新建独立线程 |
| `ThreadPool` | 用 GThreadPool（推荐）|
| `TaskGraph` | TaskGraph 系统 |
| `TaskGraphMainThread` | 主线程 TaskGraph |

## P30 - GameThread 调度

**铁律：** 后台线程 **绝不能** 直接访问 UObject 。必须 `AsyncTask(ENamedThreads::GameThread, Lambda)` 调回主线程。

```cpp
TWeakObjectPtr<AMyActor> WeakThis(this);

Async(EAsyncExecution::ThreadPool, [WeakThis]()
{
    int32 Result = HeavyWork();          // 后台线程

    AsyncTask(ENamedThreads::GameThread, [WeakThis, Result]()
    {
        AMyActor* StrongThis = WeakThis.Get();   // 重要 ：可能已销毁
        if (!StrongThis) { return; }
        StrongThis->LastResult = Result;          // 现在安全
    });
});
```

**为什么用 TWeakObjectPtr 捕获 this：** 后台 task 跑很久 ，期间 Actor 可能被销毁。WeakObjectPtr 自动失效 ，避免悬垂指针。

**FCriticalSection：**
```cpp
FCriticalSection Mutex;
{
    FScopeLock Lock(&Mutex);   // RAII 锁 ；scope 结束自动释放
    SharedData.Add(NewItem);
}
// FRWLock 适合"读多写少" ；多读者并行 + 单写者独占
```

## 测试方法

**自动化测试：** 此章节涉及 Tick / 线程 / 异步 ，单测覆盖困难 ；本项目暂未单测 P28-P30 ，靠手动 PIE 验证。

**手动 PIE：**
- P28 ：拖 `AP28_30_AsyncShowcase` 到关卡 → BP 调 StartOneShotTimer(2) → 2 秒后 TimerFireCount==1
- P29-P30 ：BP 调 RunAsyncCompute(1000) → 后台计算 1+2+...+1000=500500 → AsyncTask 调回 GameThread → LastAsyncResult=500500

## 常见坑
- 后台线程访问 UObject → 偶发崩溃（GC 同时跑）
- Timer 用 `BindLambda` 捕获 this → Actor 销毁但 Timer 未 ClearTimer → 调用悬垂 ；BindUObject 自带失效检查
- ParallelFor 里改共享数组 → 数据竞争 ；要么各自写不同 Index ，要么加锁
- Async 用 EAsyncExecution::Thread → 每次新建 OS 线程 ，开销大 ；常态用 ThreadPool
- AsyncTask 调回 GameThread 时 Actor 已销毁 → 必须 TWeakObjectPtr.Get() 检查
