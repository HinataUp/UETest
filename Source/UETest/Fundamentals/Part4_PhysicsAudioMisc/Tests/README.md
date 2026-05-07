# Part 4 自动化测试

## 文件
| 文件 | 覆盖 |
| --- | --- |
| `Part4Spec.cpp` | A/B/C/D/E 五个章节关键反射 + 默认配置 + 业务行为 |

## 输出
```
UETest.Fundamentals.P4_物理音频特效
  A_物理碰撞
    P86 SphereComp + 默认配置                       ✓
    P87 TraceHelper 是 BP Function Library          ✓
    P88 物理 Cube 默认 Simulate=true + Movable        ✓
  B_Niagara
    P89 NiagaraSpawner 是 ActorComponent             ✓
  C_音频
    P91-92 AudioHelper 静态接口反射可达               ✓
  D_Sequencer与材质
    P93 SequencePlayerHelper 是 BP Function Library  ✓
    P95 MaterialMPCHelper 是 BP Function Library     ✓
    P96 RenderTargetHelper 静态接口反射可达           ✓
  E_编辑器与打包
    P97 AddTagToActors 行为正确                      ✓
    P98 GetBuildConfiguration 不为 Unknown           ✓
    P98 IsEditorBuild + HasDebugOutput 配对正确       ✓
```

## 运行
```
Editor → Window → Developer Tools → Session Frontend → Automation
搜 UETest.Fundamentals.P4 → Start Tests
```

## 不覆盖（必须手动 PIE）
- 实际碰撞 OnHit / OnOverlap 触发
- LineTrace / Sweep / Overlap 实际命中
- 物理模拟动态行为
- Niagara System 实际播放
- Sound Cue / MetaSound 实际播放
- LevelSequence 实际播放
- RT 上 Canvas 实际绘制结果
- EUW 编辑器内运行
- Shipping 包打包验证

各章节 README "测试方法" 段给出对应步骤。
