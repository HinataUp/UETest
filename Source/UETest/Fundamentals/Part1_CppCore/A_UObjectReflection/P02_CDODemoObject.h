// P02 - CDO (Class Default Object)
//
// 每个 UClass 有且仅有一个 CDO 实例（带 RF_ClassDefaultObject 标记）
// CDO 是 NewObject 复制属性的"原型"
// 修改 CDO 会影响后续所有 NewObject 创建的新实例的初始值
//
// 访问 ：
//   GetDefault<T>()         ：const 访问
//   GetMutableDefault<T>()  ：可修改（很少使用 ；通常构造函数里设默认值更合适）

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "P02_CDODemoObject.generated.h"

UCLASS()
class UETEST_API UP02_CDODemoObject : public UObject
{
	GENERATED_BODY()

public:
	UP02_CDODemoObject();

	UPROPERTY()
	int32 TestValue;

	// 静态便捷方法 ：在测试中演示 CDO 用法
	static int32 GetCDOTestValue();
	static void SetCDOTestValue(int32 NewValue);
};
