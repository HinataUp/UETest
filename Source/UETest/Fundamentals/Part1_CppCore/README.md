# UE5 基础任务清单 · Part 1 ：C++ 核心与对象模型

## 总览
30 个任务覆盖 ：UObject 体系、指针体系、委托/事件、容器/字符串、GC/断言/日志、异步/线程/定时器。每个任务在 `Part1Spec.cpp` 中有对应自动化测试（异步/Tick 类除外）。

## 章节地图

| 章节 | 任务 | 主题 | 文件夹 |
| --- | --- | --- | --- |
| **A** | P01-P10 | UObject 体系与反射 | `A_UObjectReflection/` |
| **B** | P11-P16 | 指针体系 | `B_Pointers/` |
| **C** | P17-P20 | 委托与事件 | `C_Delegates/` |
| **D** | P21-P24 | 容器与字符串 | `D_Containers/` |
| **E** | P25-P27 | GC / 断言 / 日志 | `E_GCLog/` |
| **F** | P28-P30 | 异步 / 线程 / 定时器 | `F_Async/` |
| 公共 | — | 日志宏 | `00_Common/` |
| 测试 | — | Spec 测试 | `Tests/` |

## 任务一览

### A - UObject 体系与反射
- P01 - `NewObject` / `SpawnActor` / `CreateDefaultSubobject` 三种创建方式
- P02 - CDO（Class Default Object）
- P03 - UClass 反射 ：`StaticClass` / `GetClass` / `TFieldIterator`
- P04 - `UCLASS` Specifier（Abstract / Blueprintable / Transient / Config 等）
- P05 - `UPROPERTY` Specifier 10 种对照
- P06 - `UFUNCTION` Specifier 6 种
- P07 - `USTRUCT` 值类型 + `NetSerialize`
- P08 - `UENUM` + `UMETA`
- P09 - `UInterface` 双类模式
- P10 - `BlueprintFunctionLibrary` + `UBlueprintAsyncActionBase`

### B - 指针体系
- P11 - `TObjectPtr`（UE5 推荐）
- P12 - `TWeakObjectPtr`（弱引用，不阻止 GC）
- P13 - `TSoftObjectPtr` / `TSoftClassPtr`（软引用，存路径）
- P14 - `TSharedPtr` / `TUniquePtr` / `TWeakPtr`（非 UObject 智能指针）
- P15 - `TSubclassOf`（类型安全的 UClass 引用）
- P16 - `FGCObject`（普通 C++ 类持有 UObject 引用的标准方案）

### C - 委托与事件
- P17 - 单播委托四种绑定方式
- P18 - 多播委托 + 动态多播（BP 可见）
- P19 - `FDelegateHandle` 精确管理
- P20 - 返回值委托 + Payload 参数

### D - 容器与字符串
- P21 - `TArray` 完整算法
- P22 - `TMap` / `TSet` / `TMultiMap`
- P23 - `FString` / `FName` / `FText` 完整对比
- P24 - `TOptional` / `TVariant` / `TPair` / `TTuple`

### E - GC / 断言 / 日志
- P25 - GC 标记清除 + 根集 + 簇
- P26 - `check` / `ensure` / `verify`
- P27 - `UE_LOG` + 自定义 LogCategory

### F - 异步 / 线程 / 定时器
- P28 - `FTimerManager` 定时器
- P29 - `UE::Tasks` + `Async()` + `ParallelFor`
- P30 - GameThread 调度 + `FCriticalSection`

## 自动化测试

`UETest.Fundamentals.P1_C++核心`（Editor → Window → Developer Tools → Session Frontend → Automation）

```
A_UObject反射 ：10 个用例（P01-P10 反射可达性 + 接口 Execute_ 分发）
B_指针体系 ：3 个用例（智能指针 + FGCObject Track）
C_委托与事件 ：4 个用例（单/多播 + Handle + RetVal）
D_容器与字符串 ：4 个用例（Sort / FindOrAdd / FName 比较 / Optional）
```

详见 `Tests/README.md`。

## 学习路径建议

- 先 A 章节 ：UObject 体系是其他一切的基础
- 再 B 章节 ：理清不同指针的边界
- C / D 章节并行 ：委托 + 容器是日常 90% 用法
- E 章节最后看 ：调试武器库
- F 章节按需 ：项目用到异步时再深入

## 常见坑速查
- 详见各章节 README 末尾"常见坑"段
- 综合性问题在 `Tests/README.md`
