// P06 - UFUNCTION Specifier 全集
//
// BlueprintCallable           ：BP 可调用 ，有执行引脚
// BlueprintPure               ：BP 纯函数 ，无执行引脚 ，无副作用
// BlueprintImplementableEvent ：C++ 声明 ，BP 实现（不需要 _Implementation）
// BlueprintNativeEvent        ：C++ 提供默认 _Implementation ，BP 可覆写
// Server / Client / NetMulticast + Reliable / Unreliable / WithValidation : RPC
// Exec                         ：控制台命令函数（Pawn 上才会被识别）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P06_FunctionSpecifierActor.generated.h"

UCLASS()
class UETEST_API AP06_FunctionSpecifierPawn : public APawn
{
	GENERATED_BODY()

public:
	AP06_FunctionSpecifierPawn();

	// 1) BlueprintCallable ：有执行引脚 ；可改状态
	UFUNCTION(BlueprintCallable, Category = "P06")
	void DoAction();

	// 2) BlueprintPure ：纯函数 ；菱形节点 ；无副作用 ，可任意计算属性
	UFUNCTION(BlueprintPure, Category = "P06")
	int32 GetSquare(int32 In) const { return In * In; }

	// 3) BlueprintImplementableEvent ：C++ 声明 ，BP 实现 ；C++ 调用即可派发
	UFUNCTION(BlueprintImplementableEvent, Category = "P06")
	void OnAttackBP();

	// 4) BlueprintNativeEvent ：C++ 默认实现 + BP 可覆写
	UFUNCTION(BlueprintNativeEvent, Category = "P06")
	float GetDamageMultiplier() const;
	virtual float GetDamageMultiplier_Implementation() const { return 1.0f; }

	// 5) Server RPC ：Client 调 -> Server 执行
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendInput(int32 InputCode);

	// 6) Exec ：控制台命令 ；Pawn 上输入 "DebugRespawn" 触发
	UFUNCTION(Exec)
	void DebugRespawn();
};
