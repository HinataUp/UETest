# G 章节 - Tag 系统进阶 (G35-G37)

## 模块总览
基于 Tag 的事件驱动 ：监听 Tag 增减 → 状态切换；GE 回调 + 属性变化委托 → UI 自动更新。

| Task | 文件 | 主题 |
| --- | --- | --- |
| G35 | `G35_TagEventListener.h/cpp` | RegisterGameplayTagEvent 监听 |
| G36 | `G36_LooseVsGrantedTag.h/cpp` | Loose Tag vs GE-Driven Tag |
| G37 | `G37_GECallbacks.h/cpp` | GE 三件回调 + Attribute 变化委托 |

## 使用方法

**G35 监听 Tag 增减：**
```cpp
auto Handle = ASC->RegisterGameplayTagEvent(
        MyGameplayTags::TAG_State_Stunned,
        EGameplayTagEventType::NewOrRemoved)
    .AddUObject(this, &UMyComp::OnStunnedChanged);

// 解绑（生产代码必须做）
ASC->RegisterGameplayTagEvent(TAG, EventType).Remove(Handle);
```

**G36 Loose vs GE Tag：**
```cpp
// Loose ：手动管理 ，默认不复制
ASC->AddLooseGameplayTag(MyTag);
ASC->RemoveLooseGameplayTag(MyTag);

// 复制版本：
ASC->AddLooseGameplayTag(MyTag, 1, EGameplayTagReplicationState::TagOnly);

// GE-Driven ：GE.GrantedTags ，自动管理 + 自动复制
ASC->ApplyGameplayEffectToSelf(GE_GrantTag);
```

**G37 三种 GE 回调 + 属性委托：**
- `OnGameplayEffectAppliedDelegateToSelf` ：任何 GE 应用都触发（含 Instant）
- `OnActiveGameplayEffectAddedDelegateToSelf` ：仅 Duration/Infinite 进入 Active 时
- `OnAnyGameplayEffectRemovedDelegate` ：任意 ActiveGE 被移除
- `GetGameplayAttributeValueChangeDelegate(Attr)` ：属性变化（**UI 绑定首选**）

## 实现要点

**Loose vs GE Tag 取舍：**
| 维度 | Loose | GE-Driven |
| --- | --- | --- |
| 复制 | 默认不复制（可选 TagOnly）| 自动复制 |
| 生命周期 | 手动管理 | 自动跟随 GE |
| 性能 | 极轻 | 经过 GE 管线，稍重 |
| 适用 | 本地判定（输入缓冲、Combo Ready）| 状态 Buff（Stun、Burning）|

**GE 回调三件套时机：**
```
ApplyGameplayEffect(GE)
  → OnGameplayEffectAppliedDelegateToSelf  ← Instant 也触发
  → 只对 Duration/Infinite ：
     → OnActiveGameplayEffectAddedDelegateToSelf

GE 移除（到期 / 主动）
  → OnAnyGameplayEffectRemovedDelegate
```

**属性变化委托是 UI 绑定的首选：**
```cpp
ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetHealthAttribute())
   .AddUObject(this, &UHealthBarWidget::OnHealthChanged);

// 回调里直接拿到 OldValue / NewValue ，无需自己缓存历史
```

## 原理简述

**Tag 监听的内部实现：**
- ASC 内部维护 `FGameplayTagCountContainer`
- AddLooseGameplayTag / GE.GrantedTags 都最终走 `SetTagCount` 接口
- Tag 计数变化时触发已注册的事件回调（O(1) 查表）

**EGameplayTagEventType 三种：**
- `NewOrRemoved` ：从 0→正数 或 正数→0 时触发
- `AnyCountChange` ：任何计数变化都触发（叠层数变化也触发）

**为什么要解绑：**
- 委托句柄持有 UObject 指针
- Component 销毁时若不解绑 → ASC 端持有失效指针 → 下次回调崩
- 标准做法 ：在 EndPlay 或 Destructor 解绑

## 测试方法

**自动化测试：** `UETest.GAS.G_Tag与H_集成与I_网络`
```
G36 Loose vs Granted Tag
  - AddLooseGameplayTag 本地端可读
  - GE-Driven Tag 随 GE 生命周期
```

**手动 PIE：**
- G35 ：触发 GE_Stun3s → 日志显示 OnStunnedChanged 被调 1 次（On）；3 秒后再调 1 次（Off）
- G37 ：触发 GE_Heal30（Instant）→ 日志只见 OnApplied（不见 OnAdded）
       触发 GE_AtkBuff5s（Duration）→ OnApplied + OnAdded 都触发；5s 后 OnRemoved

## 常见坑
- 注册了 Tag 监听但 Component 销毁时不解绑 → 后续回调崩
- 期望 `OnActiveGameplayEffectAddedDelegateToSelf` 触发 Instant GE → 不会，Instant 不进 Active
- Loose Tag 在多人项目里期望复制 → 默认不复制，需要传 `EGameplayTagReplicationState::TagOnly`
- 用属性变化委托更新 UI 时 ：GAS 已经做了去重，无需自己判 OldValue==NewValue
