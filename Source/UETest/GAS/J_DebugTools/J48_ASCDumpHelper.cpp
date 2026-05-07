#include "J48_ASCDumpHelper.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "../00_Common/GASLogMacros.h"

void UASCDumpHelper::DumpASCState(UAbilitySystemComponent* ASC, const FString& Label)
{
	if (!ASC)
	{
		GAS_LOG("【Dump】ASC 为空");
		return;
	}

	FString Out;
	Out += FString::Printf(TEXT("=== ASC Dump [%s] ===\n"), *Label);

	// 1) 所有属性
	Out += TEXT("\n[Attributes]\n");
	for (const UAttributeSet* Set : ASC->GetSpawnedAttributes())
	{
		if (!Set) { continue; }
		Out += FString::Printf(TEXT("  Set: %s\n"), *Set->GetName());

		// 反射遍历 FGameplayAttributeData 字段
		for (TFieldIterator<FStructProperty> It(Set->GetClass()); It; ++It)
		{
			FStructProperty* Prop = *It;
			if (Prop->Struct == FGameplayAttributeData::StaticStruct())
			{
				const FGameplayAttributeData* Data = Prop->ContainerPtrToValuePtr<FGameplayAttributeData>(Set);
				Out += FString::Printf(TEXT("    %s : Base=%.2f Current=%.2f\n"),
					*Prop->GetName(), Data->GetBaseValue(), Data->GetCurrentValue());
			}
		}
	}

	// 2) 所有 OwnedTags
	Out += TEXT("\n[Tags]\n");
	FGameplayTagContainer Tags;
	ASC->GetOwnedGameplayTags(Tags);
	Out += FString::Printf(TEXT("  %s\n"), *Tags.ToStringSimple());

	// 3) 所有 ActiveGE
	Out += TEXT("\n[ActiveEffects]\n");
	const FActiveGameplayEffectsContainer& ActiveContainer = ASC->GetActiveGameplayEffects();
	const int32 Count = ActiveContainer.GetNumGameplayEffects();
	Out += FString::Printf(TEXT("  Count: %d\n"), Count);

	// 写文件
	const FString FilePath = FPaths::ProjectLogDir() / FString::Printf(TEXT("ASC_Dump_%s.txt"), *Label);
	FFileHelper::SaveStringToFile(Out, *FilePath);
	GAS_LOG("【Dump】已保存到 %s", *FilePath);
}
