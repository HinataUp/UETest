# B 章节 - Niagara VFX (P89-P90)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P89 | Niagara C++ 生成 + 参数控制 | `P89_NiagaraSpawner.h/cpp` |
| P90 | Niagara Data Interface（Mesh / Array）| 见下方"P90 笔记" |

## P89 - C++ Niagara

```cpp
// 一次性 ：爆炸 ；OnSystemFinished 回调
UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
    World, ExplosionSystem, Location, Normal.Rotation(), FVector(Scale),
    /*bAutoDestroy=*/true);
NC->SetVariableLinearColor(FName("Color"), FLinearColor::Red);
NC->SetVariableFloat(FName("Scale"), 2.f);
NC->OnSystemFinished.AddDynamic(this, &::HandleVFXFinished);

// 持续性 ：燃烧 ；附加到 Mesh ；不自动销毁
UNiagaraComponent* NC2 = UNiagaraFunctionLibrary::SpawnSystemAttached(
    BurningSystem, MeshComp, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
    EAttachLocation::SnapToTarget, /*bAutoDestroy=*/false);
// 结束 ：NC2->Deactivate();
```

**编辑器配 Niagara System User Parameter：**
1. Niagara System 编辑器 → User Parameters 面板
2. + 添加 ：Float / Vector / LinearColor / Object 等
3. C++ / BP 中 `SetVariableXxx(FName, Value)` 即可控制

**Bool Auto Destroy：**
- true ：System 播完自动销毁 ；适合一次性 VFX
- false ：保留 ；适合 Toggle Activate / Deactivate（角色 Buff）

## P90 - Niagara Data Interface（笔记）

**用途：** Niagara 粒子系统从外部数据源读取 ；不只局限于内部参数。

**常见 DI：**
| Data Interface | 用途 |
| --- | --- |
| Skeletal Mesh DI | 粒子从骨骼网格表面发射 |
| Static Mesh DI | 粒子从静态网格表面发射 |
| Spline DI | 粒子沿 Spline 移动 |
| Array DI | 从 C++ 传 Array 数据给 Niagara |
| Render Target DI | Niagara 写 RT / 读 RT 做反馈 |

**步骤：**
1. Niagara System 编辑器 → User Parameter 添加 `Skeletal Mesh DI`
2. C++ 中：
```cpp
UNiagaraDataInterfaceSkeletalMesh* DI = ...;
NC->SetVariableObject(FName("MeshSurface"), Mesh);
```

**实战：** 角色受击粒子从 Mesh 表面爆出 ；动态雪花覆盖物体 ；蜂群跟随路径。

## 测试方法

**自动化测试：** `UETest.Fundamentals.P4_物理音频特效 → B_Niagara`
```
P89 NiagaraSpawner 是 ActorComponent + OnExplosionFinished 委托可达
```

**手动 PIE：**
- P89 ：BP 调 `SpawnExplosion` ；屏幕看到爆炸粒子 ；调 Color=Red 看变色
- P90 ：编辑器配 Niagara System 用 Skeletal Mesh DI ；运行时 Mesh 表面发射粒子

## 常见坑
- `SetVariableXxx` 用了错误的 FName（与 Niagara System 中 User Parameter 名不一致）→ 静默失败
- bAutoDestroy=true + 想用 OnSystemFinished 控制销毁 → 顺序竞争 ；推荐 bAutoDestroy=false 自己控制
- Niagara System 性能 ：粒子数过多 → 用 GPU Sim + Cull Mode（远处不计算）
- AttachToComponent 后 Mesh 销毁 → Niagara 会自动跟着销毁 ；不需要手动清理
