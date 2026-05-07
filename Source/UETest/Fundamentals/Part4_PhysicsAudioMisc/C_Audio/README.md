# C 章节 - 音频系统 (P91-P92)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P91 | MetaSounds（UE5 新音频）| `P91_92_AudioHelper`（合并）|
| P92 | SoundCue + Attenuation + Concurrency | 同上 |

## API 速查

```cpp
// 3D 播放（带衰减 / 同时数限制）
UAudioComponent* AC = UGameplayStatics::SpawnSoundAtLocation(
    World, Sound, Location, FRotator::ZeroRotator,
    VolMul, PitchMul, /*StartTime=*/0.f, AttenuationSettings, ConcurrencySettings);

// 2D 播放（UI / 提示音）
UGameplayStatics::PlaySound2D(World, ButtonClick);

// MetaSound 参数控制（必须先 Spawn 拿到 AC）
AC->SetTriggerParameter(FName("Play"));            // 触发输入
AC->SetFloatParameter(FName("Pitch"), 1.5f);
```

## P91 - MetaSounds

**编辑器步骤：**
1. New → Sounds → MetaSound Source
2. Inputs：定义 Trigger / Float / Bool 输入
3. Graph：用节点搭信号流（Oscillator / Filter / Envelope）
4. C++ / BP 中通过 `SetTriggerParameter` / `SetFloatParameter` 实时控制

**与 SoundCue 区别：**
| 维度 | SoundCue | MetaSound |
| --- | --- | --- |
| 编辑 | 节点图（混音 / 衰减 / 选择）| 类 DSP 节点图 |
| 程序化 | 有限（Random / Modulator）| 强（运行时 DSP 构建）|
| 性能 | 已稳定 | 略高（更现代）|
| 适用 | 老项目 / 简单需求 | 新项目首选 |

UE 官方建议 ：新项目用 MetaSound。

## P92 - SoundCue + Attenuation + Concurrency

**SoundCue：** 节点图编辑 ；Random 节点（多个候选选一个）；Modulator（音量 / 音高变化）。

**USoundAttenuation 资产：**
- Inner Radius ：满音量距离
- Falloff Distance ：衰减距离
- Attenuation Shape ：球体 / 胶囊 / 盒
- Falloff Function ：Linear / Logarithmic / Inverse

**USoundConcurrency 资产：**
- MaxCount ：同类音效最大同时数（如 3 ：超过 3 个时最早的被踢出）
- Resolution Rule ：StopOldest / StopFarthest / Stop Quietest
- Volume Scale 衰减 ：超出限制后剩余声音减弱

## 测试方法

**自动化测试：** `UETest.Fundamentals.P4_物理音频特效 → C_音频`
```
P91-92 AudioHelper 静态接口反射可达（Play3D / Play2D / TriggerMetaSound / SetMetaSoundFloat）
```

**手动 PIE：**
- P91 ：MetaSound Source 资产 + 配 Trigger Input → BP 调 TriggerMetaSoundParameter 听播放
- P92 ：3D Sound + 不同距离听音量变化 ；多次 Play 看 Concurrency 限制

## 常见坑
- `SpawnSoundAtLocation` 不返回 AC ：默认 fire-and-forget ；想后控参用 `SpawnSoundAtLocation` 而非 `PlaySoundAtLocation`
- MetaSound `SetFloatParameter` 用错名字 → 静默失败
- 2D Sound 配 Attenuation → 会变成 3D ；2D 不要配
- Concurrency 配 MaxCount=1 ：单一音效永远只播 1 个 ；适合连击声 / UI 反馈
- Attenuation 形状大小不合理 → 音效"过早消失"或"全场都听到"
