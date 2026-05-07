// P03 - UClass / 反射系统
//
// StaticClass()       ：编译期获取 UClass *
// GetClass()          ：运行时获取（虚函数 ，子类返回正确类）
// FindObject<T>       ：按名字查找已加载对象
// TFieldIterator<T>   ：遍历 UPROPERTY / UFUNCTION
// FindPropertyByName  ：反射查找属性

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P03_ReflectionDemoActor.generated.h"

UCLASS()
class UETEST_API AP03_ReflectionDemoActor : public AActor
{
	GENERATED_BODY()

public:
	AP03_ReflectionDemoActor();

	UPROPERTY(EditAnywhere, Category = "P03")
	int32 IntField = 0;

	UPROPERTY(EditAnywhere, Category = "P03")
	FString StringField;

	UPROPERTY(EditAnywhere, Category = "P03")
	FVector VectorField = FVector::ZeroVector;

	// 反射遍历当前类所有 UPROPERTY ：返回数量 + 名字列表
	UFUNCTION(BlueprintCallable, Category = "P03")
	int32 CountAndDumpProperties(TArray<FString>& OutNames) const;

	// 按名字反射查找属性 ：找到则返回 true ，并把字段类型名写入 OutTypeName
	UFUNCTION(BlueprintCallable, Category = "P03")
	bool FindPropertyTypeByName(FName PropName, FString& OutTypeName) const;
};
