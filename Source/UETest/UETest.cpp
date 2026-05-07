// Copyright Epic Games, Inc. All Rights Reserved.

#include "UETest.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"
#include "Misc/Paths.h"

// 自定义模块类 ：StartupModule 中注册 shader 虚拟路径
// 让 .usf 文件能在 #include "/UETest/..." 形式下被找到
class FUETestModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();

		// 注册 shader 目录虚拟映射
		// /UETest/Private/Foo.usf -> Source/UETest/Rendering/Shaders/Private/Foo.usf
		const FString RealShaderDir = FPaths::Combine(
			FPaths::ProjectDir(),
			TEXT("Source/UETest/Rendering/Shaders"));

		if (!AllShaderSourceDirectoryMappings().Contains(TEXT("/UETest")))
		{
			AddShaderSourceDirectoryMapping(TEXT("/UETest"), RealShaderDir);
		}
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FUETestModule, UETest, "UETest");
