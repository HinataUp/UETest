// P41 - 四种 Subsystem 对比
//
// UEngineSubsystem        ：引擎级 ；进程内唯一 ；编辑器/包都存活
// UGameInstanceSubsystem  ：GameInstance 级 ；跨关卡持久（推荐放全局服务）
// UWorldSubsystem         ：World 级 ；关卡切换销毁重建
// ULocalPlayerSubsystem   ：每个本地玩家一个 ；UI / 输入相关
//
// 访问 ：
//   GEngine->GetEngineSubsystem<T>()
//   GameInstance->GetSubsystem<T>()
//   World->GetSubsystem<T>()
//   LocalPlayer->GetSubsystem<T>()

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "P41_AllSubsystems.generated.h"

UCLASS()
class UETEST_API UP41_EngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};

UCLASS()
class UETEST_API UP41_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};

UCLASS()
class UETEST_API UP41_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// World 子系统可控制是否在某些 World 类型创建（例如不在 Editor World 创建）
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
};

UCLASS()
class UETEST_API UP41_LocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
