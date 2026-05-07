#include "P02_CDODemoObject.h"

UP02_CDODemoObject::UP02_CDODemoObject()
{
	// 构造函数里设默认值 = 这就是写入 CDO 的方式
	TestValue = 10;
}

int32 UP02_CDODemoObject::GetCDOTestValue()
{
	return GetDefault<UP02_CDODemoObject>()->TestValue;
}

void UP02_CDODemoObject::SetCDOTestValue(int32 NewValue)
{
	// GetMutableDefault 拿到可写 CDO ；修改后所有新 NewObject 实例继承新值
	GetMutableDefault<UP02_CDODemoObject>()->TestValue = NewValue;
}
