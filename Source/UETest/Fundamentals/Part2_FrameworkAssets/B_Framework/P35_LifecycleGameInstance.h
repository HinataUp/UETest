// P35 - GameInstance ：跨关卡持久
// 进程内唯一 ；关卡切换不销毁 ；适合放全局数据 / 设置 / 在线服务引用
//
// 编辑器配置 ：Project Settings → Maps & Modes → Game Instance Class

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P35_LifecycleGameInstance.generated.h"

UCLASS()
class UETEST_API UP35_LifecycleGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	// 跨关卡持久数据示例 ：玩家档案
	UPROPERTY()
	FString PlayerProfileName;

	UPROPERTY()
	int32 SessionPlayCount = 0;
};
