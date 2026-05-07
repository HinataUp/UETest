#include "P58_PreprocessorDemo.h"
#include "../00_Common/FundP2LogMacros.h"

void UP58_PreprocessorHelper::EditorOnlyDebugDraw()
{
#if WITH_EDITOR
	P2_LOG("P58: WITH_EDITOR=1 ：编辑器构建运行调试绘制");
#else
	// 包构建中此分支被编译掉 ；运行时不消耗
#endif
}

bool UP58_PreprocessorHelper::IsShippingBuild()
{
#if UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}

FString UP58_PreprocessorHelper::GetPlatformName()
{
#if PLATFORM_WINDOWS
	return TEXT("Windows");
#elif PLATFORM_MAC
	return TEXT("Mac");
#elif PLATFORM_LINUX
	return TEXT("Linux");
#elif PLATFORM_IOS
	return TEXT("iOS");
#elif PLATFORM_ANDROID
	return TEXT("Android");
#else
	return TEXT("Unknown");
#endif
}
