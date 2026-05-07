# Part 2 自动化测试

## 文件
| 文件 | 覆盖 |
| --- | --- |
| `Part2Spec.cpp` | A/B/C/D/E/F/G/H 八个章节关键反射 + 默认配置 |

## 输出格式
```
UETest.Fundamentals.P2_框架与资产
  A_Actor与Component
    P31 构造函数标记 Stage_Ctor                       ✓
    P32 RootScene 已创建                              ✓
    P34 三个 Mesh 默认带 Tag                          ✓
    P34 FindMeshByTag 正确返回                        ✓
  B_Gameplay框架
    P36 GameMode.GameStateClass 绑定                  ✓
    P36 GameState bReplicates + MatchTimer            ✓
    P37 PlayerState bReplicates                        ✓
    P38 Character 暴露 IA_Move 字段                    ✓
  C_Subsystems与Tag
    P41 四个 Subsystem 类反射可达                      ✓
    P42 Tag Native 注册成功                            ✓
    P43 DamageMessage struct 字段                      ✓
  D_数据资产
    P44 PrimaryAssetId Type=Hero                      ✓
    P45 RowStruct 默认值                              ✓
  E_资源加载
    P47 SyncLoad 空路径 nullptr                        ✓
  F_存档配置本地化
    P52 SaveGame 默认值 + InventoryItem 反射            ✓
    P53 Config 类带 CLASS_Config flag                  ✓
    P54 FormatDamageText 含 Goblin/50                  ✓
  G_工程化
    P57 Pawn bReplicates                               ✓
    P58 GetPlatformName 非空                           ✓
  H_网络与关卡
    P59 NetworkBasicsActor RPC 标记正确                ✓
```

## 运行
```
Editor → Window → Developer Tools → Session Frontend → Automation
搜 UETest.Fundamentals.P2 → Start Tests
```

## 不覆盖（必须手动 PIE）
- Actor 完整生命周期（Constructor → BeginPlay → Tick → EndPlay）
- DataTable 行查询（需资产）
- 异步加载回调
- SaveGame 实际写文件
- Multiplayer RPC 实际触发
- Level Streaming 实际加载

各章节 README "测试方法" 段列出了对应手动步骤。
