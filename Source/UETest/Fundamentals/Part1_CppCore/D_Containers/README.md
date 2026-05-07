# D 章节 - 容器与字符串 (P21-P24)

## 模块总览
UE 自带容器（TArray / TMap / TSet / TMultiMap）+ 三种字符串（FString / FName / FText）+ 模式类型（TOptional / TTuple / TVariant）。

| Task | 主题 | 文件 |
| --- | --- | --- |
| P21 | TArray 完整算法 | `P21_24_ContainersLibrary` |
| P22 | TMap / TSet / TMultiMap | 同上 |
| P23 | FString / FName / FText 对比 | 同上 |
| P24 | TOptional / TTuple / TVariant | 测试中演示 |

## 容器速查

```cpp
// TArray
TArray<int32> A;
A.Add(10); A.Emplace(20); A.Insert(15, 1);
A.RemoveAt(0); A.RemoveAll([](int32 V){ return V > 100; });
A.Sort();  Algo::Reverse(A);
A.FindByPredicate([](int32 V){ return V == 20; });

// TMap (键唯一)
TMap<FName, int32> M;
M.Add(TEXT("A"), 100);
int32& Val = M.FindOrAdd(TEXT("B"));   // 不存在则 default + 引用
int32* P = M.Find(TEXT("A"));          // 不存在则 nullptr

// TSet
TSet<FName> S;
bool bNew = false;
S.Add(TEXT("X"), &bNew);
S.Contains(TEXT("X"));

// TMultiMap (键可重复)
TMultiMap<FName, int32> MM;
MM.Add(TEXT("Fire"), 1);
MM.Add(TEXT("Fire"), 2);
TArray<int32> Found;
MM.MultiFind(TEXT("Fire"), Found);   // {1, 2}
```

## 三种字符串对比

| 类型 | 内存 | 比较成本 | 修改 | 用途 |
| --- | --- | --- | --- | --- |
| **FName** | 全局表 + Index | O(1) | 不可变 | 标识符 / Tag / 资产路径 |
| **FString** | 堆分配 TCHAR* | O(N) | 可变 | 一般字符串操作 |
| **FText** | ICU 本地化包装 | O(N) | 不可变 | UI 显示文本 |

**转换：**
```cpp
FName Name(TEXT("Sword"));
FString Str = Name.ToString();
FText Txt = FText::FromString(Str);   // 失去本地化语义 ：UI 持久化避免
FText Localized = NSLOCTEXT("UI", "Damage", "造成 {0} 伤害");
FText Formatted = FText::Format(Localized, FText::AsNumber(50));
```

**经验：**
- 所有"程序内部标识"用 FName（伤害类型 / 状态名 / Tag）
- 用户输入 / 临时拼接用 FString
- UI 文本（玩家可见）用 FText ，配合 LOCTEXT 表 / .po 翻译

## TOptional / TTuple / TVariant

```cpp
TOptional<float> Maybe;
if (Maybe.IsSet()) { float V = Maybe.GetValue(); }
Maybe = 3.14f;

TTuple<FString, int32> T = MakeTuple(TEXT("A"), 1);
FString& First = T.Get<0>();

TVariant<int32, FString, float> V;
V.Set<FString>(TEXT("Hello"));
if (V.IsType<FString>()) { /* ... */ }
```

## 测试方法

**自动化测试：** `UETest.Fundamentals.P1_C++核心 → D_容器与字符串`
```
P21 TArray Sort/FindByPredicate/RemoveAll
P22 TMap FindOrAdd
P23 FName 比较 O(1)
P24 TOptional / TTuple
```

## 常见坑
- TArray 索引越界 → check 崩（Shipping 也崩）；用 `IsValidIndex` 防御
- TArray 在 for-range 中 RemoveAt → 迭代器失效 ；用 reverse 索引或 `RemoveAll`
- 拿 `Find` 返回的指针 → 后续 `Add` 可能 reallocate ，指针失效；先用值再操作
- FName 大小写不敏感 → `FName("Test") == FName("test")` 是 true（默认 IgnoreCase）
- FText::FromString 用于 UI 持久化 → 失去翻译能力 ；该用 LOCTEXT / FText::Format
