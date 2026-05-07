# Stage 4 - C++ 运行时渲染控制（T11-T13 💻 主要 C++）

## 模块总览
代码与材质的桥梁。材质负责"长什么样"，C++ 负责"什么时候 / 用什么数值"。

| Task | 主题 | 文件 |
| --- | --- | --- |
| T11 | Dynamic Material Instance（受击闪白）| `Task11_HitFlash/T11_HitFlashComponent` |
| T12 | Render Target 画布（伤痕累积）| `Task12_DamagePaint/T12_DamagePaintComponent` |
| T13 | SceneCapture（监控屏画中画）| `Task13_SceneCapture/T13_SecurityCamera` |

完成后能回答：
- MID 必须 UPROPERTY 持有，为什么？
- DrawMaterialToRenderTarget 实际开销是什么？
- SceneCapture 与普通 RT 绘制的区别？

## T11 - HitFlash Component

**用法：**
```cpp
// 角色 Mesh 已绑了一个材质，材质里预留了 HitFlashIntensity / HitFlashColor 参数
UT11_HitFlashComponent* Flash = MyChar->FindComponentByClass<UT11_HitFlashComponent>();
Flash->FlashDuration = 0.15f;
Flash->TriggerFlash(1.0f, FLinearColor::Red);  // 受击时调
```

**实现要点：**
- `BeginPlay` 一次性 `CreateDynamicMaterialInstance` → 缓存到 `UPROPERTY` 防 GC
- 用 `InitializeScalarParameterAndGetIndex` 拿 Index → 高频更新走 `SetScalarParameterByIndex`（比 ByName 快）
- 30Hz Timer 衰减 → 不浪费每帧 Tick

**关键认知：**
- `CreateDynamicMaterialInstance` 每次调用分配新 GPU 资源 → **绝不在 Tick 里调用**
- MID 是 client-local，**不能 Replicate MID 指针** → 复制驱动数值（如 bool bShouldFlash），客户端 OnRep 时本地 Trigger
- 这是最常用的"代码 ↔ 材质"桥梁

## T12 - DamagePaint Component

**用法：**
```cpp
UT12_DamagePaintComponent* Paint = MyChar->FindComponentByClass<UT12_DamagePaintComponent>();
Paint->BrushMaterial = M_RoundBrush;  // 一个圆形径向渐变材质

// 命中时
Paint->PaintDamageAtUV(FVector2D(0.5f, 0.3f), 0.05f);

// 角色身上的材质采样这张 RT 当伤痕遮罩
MyMID->SetTextureParameterValue(TEXT("DamageMask"), Paint->GetDamageRT());
```

**实现要点：**
- RT 用 `RTF_R8` 单通道 ：足够当遮罩，省内存
- `BeginPlay` 用 `CreateRenderTarget2D` + `ClearRenderTarget2D`
- `PaintDamageAtUV` 把命中坐标写入笔刷 MID → `DrawMaterialToRenderTarget` 一次全屏 pass

**关键认知：**
- RT 是 GPU 端可读写纹理 ：材质能采样，也能向它绘制
- `DrawMaterialToRenderTarget` ≈ 一次全屏 pass ：批量绘制时用 `BeginDrawCanvasToRenderTarget` + Canvas 合并
- **绝不每帧 ReadRenderTargetPixel**：会引起 GPU stall，要回读用 `FRHIGPUTextureReadback` 异步
- 典型场景 ：小地图、伤痕累积、雪地脚印、运行时纹理绘制

## T13 - SecurityCamera

**用法：**
1. 关卡放 `AT13_SecurityCamera` Actor，调整位置朝向监控区
2. 监控屏 mesh 的 MID 采样 `Camera->GetCaptureRT()` 当 BaseColor

**实现要点：**
- 构造期就关 `bCaptureEveryFrame` —— 用 Timer 间隔捕获
- 关 `Atmosphere/Fog/Bloom/SSR/MotionBlur/EyeAdaptation` 等昂贵 ShowFlags
- `GlitchChance` 模拟信号干扰 ：随机跳过本次捕获 → 画面停在上一帧

**关键认知：**
- SceneCapture 完整渲染一遍场景，是 UE 最贵渲染操作之一
- `bCaptureEveryFrame=true` 是性能陷阱
- **SceneCapture vs RT**：RT = "空白画布画东西"；SceneCapture = "把 3D 场景渲染到 RT" —— 目标都是 RT，产生方式完全不同

## 测试方法

**自动化测试：** `UETest.Rendering.渲染模块`
```
Stage 4 - T11 受击闪白
  - 默认 FlashDuration 合理
Stage 4 - T12 伤痕 RT
  - 默认 RT 尺寸为 2 的幂
Stage 4 - T13 SceneCapture 性能配置
  - CaptureComp 默认 bCaptureEveryFrame=false
  - ShowFlags Atmosphere/Bloom 关闭
```

**手动 PIE：**
- T11 ：按攻击键触发 → 角色身上闪白 0.15 秒后恢复
- T12 ：连续点几下身体 → 伤痕逐渐累积；调 ClearDamage 全清
- T13 ：放摄像头 + 监控屏 mesh → 画面每 0.5 秒更新；观察 stat unit 看 GPU 时间

## 常见坑
- T11 ：忘了 UPROPERTY 持有 MID → GC 后材质回退到原 Material
- T11 ：在 Replicate 函数里直接 SetParameterValue → 不会复制；要 Replicate 数值，OnRep 触发本地 Trigger
- T12 ：每帧调 PaintDamageAtUV → 性能塌陷；至少 100ms 节流
- T12 ：RT 没 Clear 直接用 → 初始有未定义内容
- T13 ：SceneCapture 看不到东西 → 检查 Pawn / Actor 是否在 ShowOnlyActors 列表外，或场景太暗（关了 Atmosphere 后无环境光）
