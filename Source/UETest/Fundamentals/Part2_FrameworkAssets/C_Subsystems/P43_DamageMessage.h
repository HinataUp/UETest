// P43 - GameplayMessageSubsystem ：发布 / 订阅 解耦通信
//
// 注意 ：UGameplayMessageSubsystem 来自插件 GameplayMessageRouter
//   * Project Settings → Plugins → 启用 "Gameplay Message Router"
//   * 启用后包含相关头文件即可使用
//
// 本 task 提供消息结构体 + Pub/Sub Helper ；插件未启用时 Helper 编译条件保护

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P43_DamageMessage.generated.h"

USTRUCT(BlueprintType)
struct UETEST_API FP43_DamageMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Target = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float Amount = 0.f;
};

USTRUCT(BlueprintType)
struct UETEST_API FP43_PlayerJoinedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerId = -1;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
};
