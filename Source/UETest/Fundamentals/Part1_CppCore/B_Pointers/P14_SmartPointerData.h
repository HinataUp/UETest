// P14 - 非 UObject 智能指针
//
// TSharedPtr  ：引用计数 ；可空
// TSharedRef  ：引用计数 ；不可空（断言）
// TWeakPtr    ：观察者 ；不增加引用计数
// TUniquePtr  ：独占所有权 ；不可拷贝 ，可 MoveTemp
//
// 这些是 非 UObject C++ 类 用的 ；UObject 用 UPROPERTY + TObjectPtr/TWeakObjectPtr

#pragma once

#include "CoreMinimal.h"

// 普通 C++ 类 ：不继承 UObject ，不参与 GC
class UETEST_API FP14_SmartPointerData
{
public:
	explicit FP14_SmartPointerData(int32 InValue) : Value(InValue) {}

	int32 GetValue() const { return Value; }
	void  SetValue(int32 NewValue) { Value = NewValue; }

private:
	int32 Value = 0;
};
