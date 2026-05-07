# C 章节 - 委托与事件 (P17-P20)

## 模块总览
UE 委托系统五种声明 + 四种绑定 ：单播 / 多播 / 动态多播 / 返回值 / Payload 。理清"什么时候 BP 可见"、"什么时候要 Handle"、"绑 UObject 还是 Lambda"。

| Task | 主题 | 文件 |
| --- | --- | --- |
| P17 | 单播 + 四种绑定 | `P17_20_DelegateShowcase` |
| P18 | 多播（C++）+ 动态多播（BP）| 同上 |
| P19 | FDelegateHandle / Remove / RemoveAll | 同上 |
| P20 | RetVal / Payload | 同上 |

## 速查表

| 宏 | BP 可见 | 多个绑定 | 返回值 |
| --- | --- | --- | --- |
| `DECLARE_DELEGATE` | × | × | × |
| `DECLARE_DELEGATE_RetVal` | × | × | ✓ |
| `DECLARE_MULTICAST_DELEGATE` | × | ✓ | × |
| `DECLARE_DYNAMIC_DELEGATE` | ✓ | × | × |
| `DECLARE_DYNAMIC_MULTICAST_DELEGATE` | ✓（BlueprintAssignable）| ✓ | × |
| `DECLARE_EVENT` | × | ✓（仅声明类可 Broadcast）| × |

## 五种绑定方式

```cpp
DECLARE_DELEGATE_OneParam(FOnDamage, float);
FOnDamage OnDamage;

OnDamage.BindUObject(this, &AMyActor::HandleDamage);   // UObject 方法 ；自动失效检查
OnDamage.BindRaw(RawPtr, &FStruct::Method);            // 裸指针 ；危险 ，需保证生命周期
OnDamage.BindLambda([](float V) { /* ... */ });        // Lambda
OnDamage.BindSP(SharedPtr, &FClass::Method);           // TSharedPtr 持有的对象
OnDamage.BindStatic(&FreeFunc);                        // 静态函数
OnDamage.ExecuteIfBound(50.f);
```

**动态多播只能 AddDynamic 绑 UFUNCTION：**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, Killer);
UPROPERTY(BlueprintAssignable) FOnDeath OnDeath;

OnDeath.AddDynamic(this, &AMyActor::HandleDeath);   // 必须 UFUNCTION
OnDeath.Broadcast(KillerActor);
```

## Handle 管理

```cpp
FDelegateHandle H = OnHealthChanged.AddUObject(this, &::HandleX);
OnHealthChanged.Remove(H);                  // 精确移除单次
OnHealthChanged.RemoveAll(this);            // 移除某对象的所有绑定
```

**生命周期建议：** 绑定 UObject 用 `BindUObject` ；UObject 销毁时 GAS / 委托系统自动检测失效 ，不会崩。但 RemoveAll 仍是好习惯（避免无效回调浪费）。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P1_C++核心 → C_委托与事件`
```
P17 单播 ExecuteIfBound 收到正确参数
P18 多播 Broadcast 多个绑定都触发
P19 Handle Remove 后不再触发
P20 RetVal 委托返回正确 bool
```

**手动 PIE：**
- P18 ：BP 中 Bind Event to OnDeath ，C++ Broadcast 后 BP 节点触发
- P19 ：在 Tick 中频繁 Add 不 Remove → Output Log 警告（绑定数过多）

## 常见坑
- 动态多播绑非 UFUNCTION → 编译过运行时不触发
- BindRaw 持有的对象提前销毁 → 调用时崩（裸指针无失效检查）
- Lambda 捕获 this 后 this 提前销毁 → 崩（用 TWeakObjectPtr 弱捕获）
- 多次 AddUObject 同一函数 → 触发多次回调；用 IsAlreadyBound 检查或换 Handle 重赋
