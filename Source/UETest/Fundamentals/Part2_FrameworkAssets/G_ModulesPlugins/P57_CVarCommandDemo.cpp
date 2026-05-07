#include "P57_CVarCommandDemo.h"
#include "HAL/IConsoleManager.h"
#include "../00_Common/FundP2LogMacros.h"

// CVar ：进程级全局变量 ；控制台输入 "p2.DamageMultiplier 2.0" 修改
static float GP2DamageMultiplier = 1.0f;
static FAutoConsoleVariableRef CVar_DamageMult(
	TEXT("p2.DamageMultiplier"),
	GP2DamageMultiplier,
	TEXT("【P57】全局伤害倍率（仅 Cheat）"),
	ECVF_Cheat);

// 注册纯 Console Command（非 CVar）；可绑 Lambda 执行任意逻辑
static FAutoConsoleCommand CCmd_KillAll(
	TEXT("p2.KillAll"),
	TEXT("【P57】演示 Command ：打日志（实际项目可遍历杀敌）"),
	FConsoleCommandDelegate::CreateLambda([]()
	{
		P2_LOG("P57: p2.KillAll 触发 ；当前 DamageMult=%.2f", GP2DamageMultiplier);
	}));

AP57_CVarCommandPawn::AP57_CVarCommandPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AP57_CVarCommandPawn::DebugGodMode()
{
	bGodMode = !bGodMode;
	P2_LOG("P57: GodMode = %s", bGodMode ? TEXT("ON") : TEXT("OFF"));
}

void AP57_CVarCommandPawn::DebugSetSpeed(float Speed)
{
	P2_LOG("P57: DebugSetSpeed(%.2f) [GodMode=%d]", Speed, bGodMode ? 1 : 0);
}
