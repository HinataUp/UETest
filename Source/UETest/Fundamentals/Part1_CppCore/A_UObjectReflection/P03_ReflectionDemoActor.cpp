#include "P03_ReflectionDemoActor.h"
#include "UObject/UnrealType.h"
#include "../00_Common/FundamentalsLogMacros.h"

AP03_ReflectionDemoActor::AP03_ReflectionDemoActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

int32 AP03_ReflectionDemoActor::CountAndDumpProperties(TArray<FString>& OutNames) const
{
	OutNames.Reset();

	// TFieldIterator 遍历当前类（含父类）的所有 FProperty
	for (TFieldIterator<FProperty> It(GetClass()); It; ++It)
	{
		const FProperty* Prop = *It;
		OutNames.Add(Prop->GetName());
	}
	P1_LOG("P03: %s 共有 %d 个反射属性", *GetClass()->GetName(), OutNames.Num());
	return OutNames.Num();
}

bool AP03_ReflectionDemoActor::FindPropertyTypeByName(FName PropName, FString& OutTypeName) const
{
	const FProperty* Prop = GetClass()->FindPropertyByName(PropName);
	if (!Prop) { return false; }

	// CPPType 给出可读的字段类型字符串 ：int32 / FString / FVector 等
	OutTypeName = Prop->GetCPPType();
	return true;
}
