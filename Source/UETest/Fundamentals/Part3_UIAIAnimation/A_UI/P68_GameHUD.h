// P68 - AHUD ：Canvas 绘制
//
// AHUD::DrawHUD 每帧调用 ；通过 Canvas 直接绘制 2D 元素
// 经典用法 ：屏幕中心十字准心 ；3D 位置 Project 到屏幕画头顶血条
//
// 配置 ：GameMode 的 HUDClass 设为本类

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P68_GameHUD.generated.h"

UCLASS()
class UETEST_API AP68_GameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "P68|HUD")
	float CrosshairSize = 12.f;

	UPROPERTY(EditAnywhere, Category = "P68|HUD")
	FLinearColor CrosshairColor = FLinearColor::White;
};
