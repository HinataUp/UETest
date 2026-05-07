// P09 - UInterface ：UE 接口双类声明模式
//
// UE 中接口必须用 双类 模式 ：
//   * UDamageable    继承 UInterface ：仅作为反射占位 ，不放方法
//   * IDamageable    放真正的方法 ：纯 C++ 虚函数 + UFUNCTION 接口
//
// 检查实现 ：
//   * Cast<IDamageable>(Actor)            ：原生 C++ 接口（UFUNCTION 也走这条）
//   * Actor->Implements<UDamageable>()    ：BP 实现的接口（C++ Cast 会失败）
//   * 推荐 ：用 UFUNCTION 接口 + Execute_TakeDamage 静态分发函数

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P09_DamageableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UP09_Damageable : public UInterface
{
	GENERATED_BODY()
};

class UETEST_API IP09_Damageable
{
	GENERATED_BODY()

public:
	// BlueprintNativeEvent ：C++ 默认实现 + BP 可覆写
	// UE 会生成 IP09_Damageable::Execute_TakeDamage(Object, Amount) 静态分发函数
	UFUNCTION(BlueprintNativeEvent, Category = "P09")
	float TakeDamage(float Amount);
	virtual float TakeDamage_Implementation(float Amount) { return Amount; }

	// 纯 C++ 虚函数 ：BP 不可见 ；只有 C++ 实现类需要 override
	virtual bool IsDead() const { return false; }
};
