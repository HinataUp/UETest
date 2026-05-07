// P04 - UCLASS Specifier 实战
//
// 5 个示例类 ：每个用不同 Specifier 组合 ，便于在编辑器中观察行为
// 编辑器验证点 ：
//   * Abstract 类 不能 在内容浏览器右键 "Create Blueprint Class" 实例化
//   * Blueprintable 才允许创建蓝图子类
//   * Config=Game 的属性会从 DefaultGame.ini 加载
//   * Within=AOuter 限制本类只能作为指定 Outer 的 Subobject

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "P04_ClassSpecifierShowcase.generated.h"

// 1) Abstract ：基类，不能直接实例化 ；常用作策略基类
UCLASS(Abstract, Blueprintable)
class UETEST_API UP04_AbstractBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void DoStrategy();
};

// 2) Blueprintable + BlueprintType ：允许 BP 子类化 + 可作为 BP 变量类型
UCLASS(Blueprintable, BlueprintType)
class UETEST_API UP04_Blueprintable : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "P04")
	int32 Value = 0;
};

// 3) NotBlueprintable ：明确禁止蓝图子类化（一般引擎核心类用）
UCLASS(NotBlueprintable)
class UETEST_API UP04_NotBlueprintable : public UObject
{
	GENERATED_BODY()
};

// 4) Transient ：实例化后不参与存盘（不会写到 Save Game / Map）
UCLASS(Transient)
class UETEST_API UP04_Transient : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 RuntimeOnlyValue = 0;
};

// 5) Config=Game ：属性从 Saved/Config/DefaultGame.ini 加载
//    UPROPERTY(Config) 标记的字段才会从 INI 读取
UCLASS(Config = Game)
class UETEST_API UP04_ConfigClass : public UObject
{
	GENERATED_BODY()
public:
	// .ini 中 ：[/Script/UETest.P04_ConfigClass] MaxPlayers=8
	UPROPERTY(Config)
	int32 MaxPlayers = 4;
};
