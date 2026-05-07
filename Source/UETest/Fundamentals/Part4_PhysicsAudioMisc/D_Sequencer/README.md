# D 章节 - Sequencer / PCG / Materials / RT (P93-P96)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P93 | Sequencer C++ 播放 | `P93_SequencePlayer.h/cpp` |
| P94 | PCG 程序化生成 | 见下方"P94 笔记" |
| P95 | Dynamic Material Instance + MPC | `P95_MaterialMPCHelper.h/cpp` |
| P96 | Render Target + Canvas 绘制 | `P96_RenderTargetHelper.h/cpp` |

## P93 - Sequencer

```cpp
ALevelSequenceActor* SeqActor = UP93_SequencePlayerHelper::PlayLevelSequence(this, MySequence);
if (SeqActor && SeqActor->GetSequencePlayer()) {
    SeqActor->GetSequencePlayer()->OnFinished.AddDynamic(this, &::HandleSeqDone);
}
UP93_SequencePlayerHelper::PauseSequence(SeqActor);
UP93_SequencePlayerHelper::StopSequence(SeqActor);
```

**编辑器步骤：**
1. New → Animation → Level Sequence
2. Sequencer 编辑器内拖入 Camera / Actor → 加 Track（位置 / 旋转 / 可见 / Animation）
3. 关键帧动画轨道
4. 运行时通过 Helper 播放

**实战：** 过场动画 / 镜头切换 / 角色介绍 / 关卡开场。

## P94 - PCG（笔记）

**Plugin：** Procedural Content Generation Framework

**核心：**
- `UPCGComponent` ：挂在 Actor 上 ；执行 Graph 生成内容
- `UPCGGraph` ：节点图 ；输入 Surface / 数据 → 输出 Spawn Mesh / Actor

**典型节点链：**
```
Get Actor Data  →  Surface Sampler  →  Filter By Slope  →  Mesh Spawner
   (本 Actor)      (在 Surface 采点)    (按坡度过滤)        (放树 / 草)
```

**步骤：**
1. New → PCG → PCG Graph
2. 拖入采样节点 ：Surface Sampler / Volume Sampler
3. Filter ：按 Tag / Slope / Distance
4. Output ：Mesh Spawner / Static Mesh Spawner / Spawn Actor
5. 关卡 Actor 加 PCGComponent → 选 Graph → 自动执行

**C++ 自定义节点：** 继承 `UPCGSettings`（Settings + Element 双类）。

## P95 - MID + MPC

```cpp
// MID ：每实例自定义
UMaterialInstanceDynamic* MID = UP95_MaterialMPCHelper::CreateMIDAndSet(
    Mesh, /*SlotIndex=*/0, FName("EmissiveStrength"), 5.f);

// MPC ：跨材质共享
UP95_MaterialMPCHelper::SetMPCScalar(this, WeatherMPC, FName("Wetness"), 0.8f);
```

**MID vs MPC 边界：**
| 维度 | MID | MPC |
| --- | --- | --- |
| 范围 | 单实例 | 全局 |
| 创建 | `CreateDynamicMaterialInstance` | 编辑器创建 MPC 资产 |
| 用途 | 受击闪白 / 角色染色 | 天气 / 时间 |

详见 Rendering 模块 T11 / T04。

## P96 - Render Target

```cpp
UTextureRenderTarget2D* RT = UP96_RenderTargetHelper::CreateRT(this, 512);
UP96_RenderTargetHelper::DrawCircleOnRT(this, RT, FVector2D(256, 256), 50.f, FLinearColor::Red);
UP96_RenderTargetHelper::ClearRT(this, RT, FLinearColor::Black);

// 让材质采样这张 RT
MID->SetTextureParameterValue(FName("DamageMask"), RT);
```

**API：** `BeginDrawCanvasToRenderTarget` → 用 Canvas API 绘制 → `EndDrawCanvasToRenderTarget`

**实战：** 小地图 / 动态贴花 / 雪地脚印 / 伤痕累积（详见 Rendering T12）。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P4_物理音频特效 → D_Sequencer与材质`
```
P93 SequencePlayerHelper 是 BP Function Library
P95 MaterialMPCHelper 是 BP Function Library
P96 RenderTargetHelper 静态接口反射可达
```

**手动 PIE：**
- P93 ：放 LevelSequence 资产 → BP 调 PlayLevelSequence → 镜头按 Sequence 移动
- P95 ：BP 调 CreateMIDAndSet 看材质即时变化 ；调 SetMPCScalar 看全场材质同步
- P96 ：调 DrawCircleOnRT → 场景中采样 RT 的材质显示红圆

## 常见坑
- `LevelSequencePlayer::Play()` 没生效 → 检查 PlaybackSettings.bPauseAtEnd / bRestoreState
- MID 不持有 UPROPERTY → GC 后失效 ；本 Helper 返回的 MID 必须存到调用方 UPROPERTY
- MPC 改变后部分材质未刷新 → MPC 改 Scalar 是即刻的 ；如果材质里用 Switch 静态分支 ，需要 Static Switch 不能用 MPC
- Canvas 绘制不够精确 → Canvas 是 GPU 1 个 Quad pass ；高精度图形用 DrawMaterialToRenderTarget + 自己写材质
- BeginDrawCanvasToRenderTarget 后忘了 End → RT 资源泄漏 + 后续 Draw 异常
