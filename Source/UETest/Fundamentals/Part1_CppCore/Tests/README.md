# Part 1 自动化测试

## 文件
| 文件 | 覆盖 |
| --- | --- |
| `Part1Spec.cpp` | A/B/C/D 章节核心断言（GC/线程靠手动 PIE）|

## 输出格式
```
UETest.Fundamentals.P1_C++核心
  A_UObject反射
    P01 NewObject 应返回有效指针            ✓
    P02 CDO 与实例不同                      ✓
    P03 反射可遍历 UPROPERTY                ✓
    P04 Abstract 类带 CLASS_Abstract flag   ✓
    P05 J_Replicated 已注册到 RepProps       ✓
    P06 ServerSendInput 反射 + 标记正确      ✓
    P07 USTRUCT 拷贝独立                    ✓
    P08 UENUM 反射可达 + uint8 底层         ✓
    P09 接口 Implements + Cast              ✓
    P10 BP 函数库 SquareInt                 ✓
  B_指针体系
    P14 TSharedPtr 引用计数                 ✓
    P14 TUniquePtr 不可拷贝可 Move           ✓
    P16 FGCObject 能 Track UObject          ✓
  C_委托与事件
    P17 单播 ExecuteIfBound                 ✓
    P18 多播 Broadcast 多个绑定             ✓
    P19 Handle Remove                       ✓
    P20 RetVal 委托                         ✓
  D_容器与字符串
    P21 TArray 综合                          ✓
    P22 TMap FindOrAdd                       ✓
    P23 FName 比较 O(1)                      ✓
    P24 TOptional / TTuple                   ✓
```

## 运行
**UI：** Window → Developer Tools → Session Frontend → Automation → 搜 `UETest.Fundamentals.P1` → Start Tests

**命令行：**
```
UnrealEditor-Cmd UETest.uproject \
  -ExecCmds="Automation RunTests UETest.Fundamentals; Quit" \
  -unattended -nullrhi -log
```

## 不覆盖
GC 实际回收 / 异步线程 / Timer / SoftObjectPtr 实际加载 — 都需要 World/PIE 上下文，本 Spec 不写。详见各章节 README "测试方法" 段的手动 PIE 步骤。
