// P16 - FGCObject ：非 UObject 持有 UObject 引用
//
// 痛点 ：普通 C++ 类持有 UObject* ，GC 不知道 → UObject 可能被回收 → 悬垂指针
// 解法 ：继承 FGCObject ，override AddReferencedObjects 把指针报告给 GC

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"

class UObject;

class UETEST_API FP16_FGCObjectManager : public FGCObject
{
public:
	FP16_FGCObjectManager() = default;

	// 添加被管理对象 ：之后这些 UObject 不会被 GC 回收
	void Track(UObject* Obj);

	// FGCObject API
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FP16_FGCObjectManager");
	}

	// 暴露给测试 ：返回当前管理对象数
	int32 GetTrackedNum() const { return ManagedObjects.Num(); }
	UObject* GetTrackedAt(int32 Index) const { return ManagedObjects.IsValidIndex(Index) ? ManagedObjects[Index] : nullptr; }

private:
	// 注意 ：这里用裸指针 而不是 UPROPERTY ；GC 不会自动扫描 ，必须 AddReferencedObjects 手动报告
	TArray<UObject*> ManagedObjects;
};
