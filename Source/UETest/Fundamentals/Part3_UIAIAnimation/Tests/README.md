# Part 3 自动化测试

## 文件
| 文件 | 覆盖 |
| --- | --- |
| `Part3Spec.cpp` | A/B/C 三个章节关键反射 + 默认配置 |

## 输出
```
UETest.Fundamentals.P3_UI_AI_动画
  A_UI
    P61 BindWidget 字段反射可达                      ✓
    P62 OnStartGameRequested 委托可达                 ✓
    P63 Slot Widget 实现 IUserObjectListEntry         ✓
    P68 GameHUD 默认配置                              ✓
  B_AI
    P70 BTTask_FindTarget 是 BTTaskNode 子类         ✓
    P70 BTDecorator 默认 Range 合理                  ✓
    P72 PerceptionComp + SightConfig 已创建          ✓
    P73 NavMeshHelper 是 BlueprintFunctionLibrary    ✓
  C_动画
    P78 AnimInstance 暴露 Speed/Direction/bIsInAir   ✓
    P80 Notify / NotifyState 子类                     ✓
    P83 ControlRigHelper 是 ActorComponent           ✓
```

## 运行
```
Editor → Window → Developer Tools → Session Frontend → Automation
搜 UETest.Fundamentals.P3 → Start Tests
```

## 不覆盖（必须手动 PIE）
- UMG Widget 实际渲染 / BindWidget 实际填充
- Behavior Tree 实际执行
- AI Perception 实际感知
- NavMesh 实际寻路
- AnimInstance NativeUpdateAnimation 实际驱动 BlendSpace
- AnimNotify 实际触发
- Control Rig 实际控制骨骼

各章节 README 给出了对应手动验证步骤。
