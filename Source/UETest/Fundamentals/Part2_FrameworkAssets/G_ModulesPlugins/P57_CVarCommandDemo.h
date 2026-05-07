// P57 - Console Variable + Console Command + Exec UFUNCTION
//
// CVar      ：FAutoConsoleVariableRef ；通常 ECVF_Cheat 防止玩家滥用
// Command   ：IConsoleManager::Get().RegisterConsoleCommand
// Exec      ：UFUNCTION(Exec) 在 Pawn / PlayerController / GameMode 上识别

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P57_CVarCommandDemo.generated.h"

UCLASS()
class UETEST_API AP57_CVarCommandPawn : public APawn
{
	GENERATED_BODY()

public:
	AP57_CVarCommandPawn();

	// Exec ：控制台输入函数名直接调用
	UFUNCTION(Exec)
	void DebugGodMode();

	UFUNCTION(Exec)
	void DebugSetSpeed(float Speed);

	bool bGodMode = false;
};
