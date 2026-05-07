#include "P21_24_ContainersLibrary.h"
#include "Algo/Reverse.h"
#include "Internationalization/Text.h"
#include "../00_Common/FundamentalsLogMacros.h"

void UP21_24_ContainersLibrary::DemoTArray(TArray<int32>& InOutArray)
{
	// Add ：拷贝插入
	InOutArray.Add(99);
	// Emplace ：原地构造（容器内对象类型有重构造时更高效）
	InOutArray.Emplace(100);
	// Sort
	InOutArray.Sort();
	// FindByPredicate
	int32* Found = InOutArray.FindByPredicate([](int32 V) { return V == 100; });
	// RemoveAll ：按谓词批量移除
	InOutArray.RemoveAll([](int32 V) { return V > 90 && V < 95; });
	// Algo::Reverse
	Algo::Reverse(InOutArray);

	P1_LOG("P21: TArray Demo 后大小 = %d, Found100 = %d",
		InOutArray.Num(), Found ? *Found : -1);
}

int32 UP21_24_ContainersLibrary::DemoTMap(TMap<FName, int32>& InOutMap, FName Key)
{
	// FindOrAdd ：键存在则返回引用 ，不存在则添加默认值并返回引用
	int32& Value = InOutMap.FindOrAdd(Key);
	Value += 10;
	return Value;
}

FString UP21_24_ContainersLibrary::DemoStringTypes(FName InName, FText& OutText)
{
	// FName -> FString
	const FString Str = InName.ToString();

	// FString -> FText（注意丢失本地化信息 ；不要在 UI 持续显示文本上用）
	OutText = FText::FromString(Str);

	// FText 格式化 ：本地化项目首选
	const FText Formatted = FText::Format(
		NSLOCTEXT("P23", "Demo", "你好 {0} ！"),
		OutText);

	P1_LOG("P23: FName=%s | FString=%s | FText=%s",
		*InName.ToString(), *Str, *Formatted.ToString());

	return Str;
}
