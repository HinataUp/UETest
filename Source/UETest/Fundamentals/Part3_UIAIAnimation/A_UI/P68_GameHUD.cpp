#include "P68_GameHUD.h"
#include "Engine/Canvas.h"

void AP68_GameHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas) { return; }

	// 屏幕中心
	const float CenterX = Canvas->SizeX * 0.5f;
	const float CenterY = Canvas->SizeY * 0.5f;
	const float Half = CrosshairSize * 0.5f;

	// 横线
	DrawLine(CenterX - Half, CenterY, CenterX + Half, CenterY, CrosshairColor, 2.f);
	// 竖线
	DrawLine(CenterX, CenterY - Half, CenterX, CenterY + Half, CrosshairColor, 2.f);

	// 演示 Canvas->Project ：把世界坐标投影到屏幕
	// 实战 ：用此函数把敌人头顶 World Location -> Screen XY ，然后 DrawText / DrawRect 画血条
}
