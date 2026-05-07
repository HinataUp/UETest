# A 章节 - UObject 体系与反射 (P01-P10)

## 模块总览
UE C++ 的"操作系统"层 ：UObject 生命周期、CDO、UClass 反射、UCLASS / UPROPERTY / UFUNCTION / USTRUCT / UENUM / UInterface 全 Specifier。

| Task | 主题 | 文件 |
| --- | --- | --- |
| P01 | UObject 生命周期（NewObject/SpawnActor/CreateDefaultSubobject）| `P01_LifecycleObject.h/cpp` |
| P02 | CDO（Class Default Object）| `P02_CDODemoObject.h/cpp` |
| P03 | UClass 反射（StaticClass/GetClass/TFieldIterator）| `P03_ReflectionDemoActor.h/cpp` |
| P04 | UCLASS Specifier 5 个示例 | `P04_ClassSpecifierShowcase.h/cpp` |
| P05 | UPROPERTY Specifier 10 个字段对照 | `P05_PropertySpecifierActor.h/cpp` |
| P06 | UFUNCTION Specifier 6 种 | `P06_FunctionSpecifierActor.h/cpp` |
| P07 | USTRUCT + NetSerialize | `P07_DamageInfoStruct.h/cpp` |
| P08 | UENUM + UMETA | `P08_WeaponTypeEnum.h` |
| P09 | UInterface 双类模式 | `P09_DamageableInterface.h/cpp` + `P09_DamageableActor.h/cpp` |
| P10 | BlueprintFunctionLibrary + AsyncActionBase | `P10_BlueprintLibrary.h/cpp` |

## 关键认知

**P01 - 三种创建方式：**
| API | 何时用 | 说明 |
| --- | --- | --- |
| `NewObject<T>(Outer)` | 所有非 Actor UObject | Outer 决定生命周期 |
| `SpawnActor<T>(Class, ...)` | Actor | 内部仍调 NewObject + 注册到 World |
| `CreateDefaultSubobject<T>` | **构造函数中** 创建组件 | 运行时改用 NewObject + RegisterComponent |

**P02 - CDO：** 每个 UClass 一个 CDO（带 `RF_ClassDefaultObject` 标记），是 NewObject 复制属性的"原型"。改 CDO 等于改默认值——通常通过构造函数赋值更直观。

**P03 - 反射：**
```cpp
for (TFieldIterator<FProperty> It(GetClass()); It; ++It) { /* 遍历所有属性 */ }
FProperty* P = GetClass()->FindPropertyByName(TEXT("FieldName"));
P->GetCPPType();  // 返回 "int32" / "FString" 等
```

**P05 - UPROPERTY 编辑器可见性 4 件套：**
| Specifier | 编辑权限 |
| --- | --- |
| EditAnywhere | BP CDO + 关卡实例 + 派生 BP 都可编辑 |
| EditDefaultsOnly | 只 BP CDO 可编辑（关卡实例灰色）|
| EditInstanceOnly | 只关卡实例可编辑（BP CDO 灰色）|
| VisibleAnywhere | 所有地方只读 |

**P09 - 接口双类：**
```cpp
UINTERFACE(MinimalAPI, Blueprintable)
class UMyInterface : public UInterface {};   // 反射占位

class IMyInterface {
    UFUNCTION(BlueprintNativeEvent) float TakeDamage(float Amount);
    virtual float TakeDamage_Implementation(float Amount) { return Amount; }
    virtual bool IsDead() const = 0;   // 纯 C++，BP 不可见
};
```
检查 ：`Cast<IMyInterface>(Actor)`（C++ 实现）/ `Actor->Implements<UMyInterface>()`（含 BP 实现）/ 推荐 `IMyInterface::Execute_TakeDamage(Actor, 30.f)` 静态分发。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P1_C++核心 → A_UObject反射`
```
P01 NewObject 应返回有效指针
P02 CDO 与实例不同 + RF_ClassDefaultObject 标记 + 写入影响新实例
P03 反射可遍历 UPROPERTY + FindPropertyByName
P04 Abstract 类带 CLASS_Abstract flag
P05 J_Replicated 已注册到 RepProps
P06 ServerSendInput 反射 + FUNC_NetServer/Reliable/Validate 标记
P07 USTRUCT 拷贝独立
P08 UENUM 反射可达 + uint8 底层
P09 接口 Implements + Cast + Execute_ 静态分发
P10 BP 函数库 SquareInt
```

**手动 PIE：**
- P04 ：尝试 BP 子类化 Abstract 类应失败；Config 类的属性从 DefaultGame.ini 加载
- P05 ：在 Details 面板逐一对照 10 个属性的编辑权限
- P06 ：BlueprintNativeEvent BP 子类覆写后 ，C++ 调用走子类版本
- P10 ：BP 中搜索到 SquareInt / Async Wait Ticks 节点

## 常见坑
- 运行时调 `CreateDefaultSubobject` → 崩；只能在构造函数里
- CDO 修改后忘了还原 → 影响后续测试
- 接口忘了双类 → 无 UFUNCTION 不能 BP 实现
- USTRUCT 漏 NetSerialize → 网络复制时用默认逐字段序列化（体积可能很大）
