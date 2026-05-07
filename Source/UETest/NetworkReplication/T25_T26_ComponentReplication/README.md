# T25-T26 组件复制

## 模块总览
Component 是 Actor 的"零件"，本身也能拥有 Replicated 字段，是组合化代码的关键。本目录覆盖：常规 Replicated Component + UE5 推荐的 ReplicatedSubObject（GAS 同款机制）。

| Task | 主题 | 文件 |
|---|---|---|
| T25 | Replicated Component 基础 | `T25_RepCombatComponent` + `AT25_RepComponentActor` |
| T26 | ReplicatedSubObject（GAS 风格）| `UT26_RepSubObject` + `AT26_ReplicatedSubObjectActor` |

完成后你能回答：
- Component 与 Actor 的复制开关有什么关系？
- 普通 UObject 怎么靠 ReplicatedSubObject 跨端？
- GAS 的 AttributeSet 为什么能同步？

## 使用方法

**T25：**
1. 关卡放一个 `AT25_RepComponentActor`
2. PIE → 三端日志能看到 `CompValue` 同步增长

**T26：**
1. 关卡放一个 `AT26_ReplicatedSubObjectActor`
2. PIE → 0.5 秒后 Server NewObject 出 SubObj 并 AddReplicatedSubObject
3. 之后每 2 秒 Server 改 `SubObj->SubObjValue` → Client 端能跟随

## 实现要点

**T25 两个开关都要开：**
```cpp
// Actor 级别
bReplicates = true;

// Component 级别（UE5 推荐写法）
SetIsReplicatedByDefault(true);
// 等价于 BP 中勾选 Component Replicates
```

**Component 的 GetLifetimeReplicatedProps：**
```cpp
void UMyComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutProps) const
{
    Super::GetLifetimeReplicatedProps(OutProps);
    DOREPLIFETIME(UMyComp, MyValue);   // 注意：Component 自己写自己的
}
```

**T26 ReplicatedSubObject（UE5 新 API）：**
```cpp
// 1) UObject 必须 override IsSupportedForNetworking
class UMySubObj : public UObject
{
    virtual bool IsSupportedForNetworking() const override { return true; }
    UPROPERTY(Replicated) int32 Value;
};

// 2) 拥有它的 Actor 启用注册式列表
MyActor::MyActor()
{
    bReplicateUsingRegisteredSubObjectList = true;
}

// 3) Server 创建后注册
SubObj = NewObject<UMySubObj>(this);
AddReplicatedSubObject(SubObj);
```

**新 API vs 旧 API 对比：**
| 方式 | UE 版本 | 风格 |
|---|---|---|
| `AddReplicatedSubObject` + `bReplicateUsingRegisteredSubObjectList` | UE5.1+ | 注册式，无需 override |
| `override ReplicateSubobjects(Channel, Bunch, RepFlags)` | UE4 / UE5 兼容 | 手动驱动 |
| 二者**互斥**，只能选一种 |

## 原理简述

**Component 复制的两步握手：**
- `bReplicates`（Actor）决定整个 Actor 是否参与复制
- `SetIsReplicated`（Component）决定 Component 上的字段是否参与
- 任一关闭，Component 字段不同步

**为什么 DefaultSubobject Component 能跨端：**
- `CreateDefaultSubobject` 创建的 Component 有稳定 NetGUID（与 Actor 一起对齐）
- Client 加载 Actor 时同步构造同名 Component
- 所以指针引用可解析、字段可同步

**ReplicatedSubObject 的本质：**
- 普通 UObject 没自带网络身份
- 但如果 Actor 在复制时**显式声明"我还附带一个子对象，请帮我也复制它"**，UE 就把这个 UObject 当作 Actor 的"扩展"一起处理
- 子对象的 NetGUID 由"宿主 Actor + 子对象唯一标识"组合产生

**GAS 的玩法：**
- `UAbilitySystemComponent` 是个 Replicated Component
- 它内部装着多个 `UAttributeSet`（普通 UObject）和 `UGameplayAbility`（普通 UObject）
- 通过 `ReplicateSubobjects` / `AddReplicatedSubObject` 把它们一并发给 Client
- 所以 Client 上能看到属性变化、能执行能力

## 测试方法

**手动 PIE：**
- T25：Component 字段同步速度与普通 Replicated 字段无差别
- T26：观察 Server 第 0.5 秒 NewObject 后，Client 是否能在第 1-2 秒看到 SubObjValue=1

**自动化测试：** `UETest.Network.生命周期与组件`

```
覆盖：
  T25 Component 默认开启复制 (SetIsReplicatedByDefault)
     CompValue 字段已注册
     Actor 默认带有 Component (CreateDefaultSubobject)
  T26 Actor 启用了注册式子对象复制
     UT26_RepSubObject::IsSupportedForNetworking 返回 true
     SubObjValue 字段已注册
```

## 常见坑
- 只开 Actor `bReplicates` 没开 Component `SetIsReplicated` → Component 字段不同步
- Component 忘写自己的 `GetLifetimeReplicatedProps` → 字段不同步且无 warning
- 运行时 `AddComponent` 想复制 → 默认无 NetGUID，Client 上不存在；建议改成 DefaultSubobject
- 同时用 `bReplicateUsingRegisteredSubObjectList=true` 又 override `ReplicateSubobjects` → 行为冲突，新 API 会忽略旧实现
- ReplicatedSubObject 的 SubObject 没 override `IsSupportedForNetworking` → 复制失败
