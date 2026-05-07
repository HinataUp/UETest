// P53 - Config 系统 ：UCLASS(config=Game) + UPROPERTY(Config)
//
// 编辑器配置 ：DefaultGame.ini 的 [/Script/<模块>.<类名>] section
// 例 ：
//   [/Script/UETest.P53_MySettingsConfig]
//   MasterVolume=0.8
//   ServerIP=192.168.1.100

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "P53_MySettingsConfig.generated.h"

UCLASS(Config = Game, DefaultConfig)
class UETEST_API UP53_MySettingsConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "P53|Settings")
	float MasterVolume = 1.0f;

	UPROPERTY(Config, EditAnywhere, Category = "P53|Settings")
	FString ServerIP = TEXT("127.0.0.1");

	UPROPERTY(Config, EditAnywhere, Category = "P53|Settings")
	int32 MaxFPS = 60;
};
