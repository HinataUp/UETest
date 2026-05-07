# B 章节 - 指针体系 (P11-P16)

## 模块总览
UE 中 6 种指针类型的边界 ：UObject 用 TObjectPtr/TWeakObjectPtr/TSoftObjectPtr，非 UObject 用 TSharedPtr/TUniquePtr。FGCObject 是"非 UObject 类持有 UObject"的标准方案。

| Task | 主题 | 文件 |
| --- | --- | --- |
| P11 | TObjectPtr | `P11_15_PointerShowcaseActor` |
| P12 | TWeakObjectPtr | 同上 |
| P13 | TSoftObjectPtr / TSoftClassPtr | 同上 |
| P14 | TSharedPtr / TUniquePtr / TWeakPtr | `P14_SmartPointerData` |
| P15 | TSubclassOf | 同上（P11-15 合并 Showcase）|
| P16 | FGCObject | `P16_FGCObjectManager` |

## 速查表

| 指针 | UObject? | 阻止 GC | 适用场景 |
| --- | --- | --- | --- |
| `TObjectPtr<T>` | ✓ | ✓ | UPROPERTY 成员 ；UE5 推荐 |
| 裸指针 `T*` | ✓ | × | 局部变量 / 函数参数 |
| `TWeakObjectPtr<T>` | ✓ | × | 想引用但不阻止 GC（缓存）|
| `TSoftObjectPtr<T>` | ✓ | × | 资产路径 ；运行时 LoadSynchronous |
| `TSubclassOf<T>` | ✓ | ✓ | 限制 UClass 类型 ；编辑器下拉过滤 |
| `TSharedPtr<T>` | × | — | 非 UObject 引用计数 |
| `TUniquePtr<T>` | × | — | 非 UObject 独占所有权 |
| `TSharedRef<T>` | × | — | TSharedPtr 不可空版（断言）|
| `FGCObject` | × | ✓ | 普通 C++ 类持有 UObject 时手动报告 GC |

## 关键代码

**P11 - TObjectPtr：**
```cpp
UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> MeshComp;  // UE5
// 等价于裸指针 + 运行时跟踪 ；非 UPROPERTY 局部变量仍可裸指针
```

**P12 - TWeakObjectPtr：**
```cpp
TWeakObjectPtr<AActor> Cached = SomeActor;
if (Cached.IsValid()) { Cached->DoSomething(); }
// SomeActor 被 Destroy 后 Cached.IsValid() == false ，安全
```

**P13 - TSoftObjectPtr：**
```cpp
UPROPERTY(EditAnywhere) TSoftObjectPtr<UStaticMesh> SoftMesh;
UStaticMesh* M = SoftMesh.LoadSynchronous();    // 阻塞加载
// 异步用 FStreamableManager::RequestAsyncLoad
```

**P14 - 智能指针：**
```cpp
TSharedPtr<FMyData> A = MakeShared<FMyData>(42);
TWeakPtr<FMyData> Weak = A;          // 不影响引用计数
TUniquePtr<FMyData> U = MakeUnique<FMyData>(99);
TUniquePtr<FMyData> M = MoveTemp(U); // U 失去所有权
```

**P16 - FGCObject：**
```cpp
class FMyMgr : public FGCObject {
    TArray<UObject*> Tracked;
    virtual void AddReferencedObjects(FReferenceCollector& C) override
    { C.AddReferencedObjects(Tracked); }
    virtual FString GetReferencerName() const override { return TEXT("FMyMgr"); }
};
```

## 测试方法

**自动化测试：** `UETest.Fundamentals.P1_C++核心 → B_指针体系`
```
P14 TSharedPtr 引用计数共享
P14 TUniquePtr 不可拷贝可 Move
P16 FGCObject 能 Track UObject
```

**手动 PIE：**
- P12 ：Spawn Actor → 存 TWeakObjectPtr → Destroy → ForceGarbageCollection → IsValid() == false
- P13 ：编辑器选 Mesh 资产 → 运行时 IsValid()=false（未加载）→ LoadSynchronous → IsValid()=true

## 常见坑
- UPROPERTY 中混用裸指针 + TObjectPtr → 性能 / 一致性问题；UE5 全用 TObjectPtr
- 普通 C++ 类持 UObject* 而不继承 FGCObject → UObject 被 GC，悬垂指针崩
- TWeakPtr 访问前不 IsValid() → 崩
- TSoftObjectPtr 期望"自动加载"→ 必须显式 Load
