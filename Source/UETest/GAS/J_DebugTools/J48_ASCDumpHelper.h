// J48 - ASC 状态 Dump 工具
// 一键导出 ASC 完整状态到文件 ，用于三端 Diff 排查同步 Bug

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "J48_ASCDumpHelper.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UETEST_API UASCDumpHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// 把 ASC 当前状态写到 ProjectLogDir/ASC_Dump_<Label>.txt
	UFUNCTION(BlueprintCallable, Category = "GAS|Debug")
	static void DumpASCState(UAbilitySystemComponent* ASC, const FString& Label);
};
