// Part 1 - C++ 核心与对象模型 自动化测试

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UObject/UnrealType.h"

#include "../A_UObjectReflection/P01_LifecycleObject.h"
#include "../A_UObjectReflection/P02_CDODemoObject.h"
#include "../A_UObjectReflection/P03_ReflectionDemoActor.h"
#include "../A_UObjectReflection/P04_ClassSpecifierShowcase.h"
#include "../A_UObjectReflection/P05_PropertySpecifierActor.h"
#include "../A_UObjectReflection/P06_FunctionSpecifierActor.h"
#include "../A_UObjectReflection/P07_DamageInfoStruct.h"
#include "../A_UObjectReflection/P08_WeaponTypeEnum.h"
#include "../A_UObjectReflection/P09_DamageableInterface.h"
#include "../A_UObjectReflection/P09_DamageableActor.h"
#include "../A_UObjectReflection/P10_BlueprintLibrary.h"

#include "../B_Pointers/P14_SmartPointerData.h"
#include "../B_Pointers/P16_FGCObjectManager.h"

#include "../C_Delegates/P17_20_DelegateShowcase.h"

#include "../D_Containers/P21_24_ContainersLibrary.h"

BEGIN_DEFINE_SPEC(
	FUETest_FundamentalsP1_Spec,
	"UETest.Fundamentals.P1_C++核心",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_FundamentalsP1_Spec)

void FUETest_FundamentalsP1_Spec::Define()
{
	// ───────────────────── A 章节 ─────────────────────
	Describe(TEXT("A_UObject反射"), [this]()
	{
		It(TEXT("P01 NewObject 应返回有效指针"), [this]()
		{
			UP01_PlainObject* Obj = NewObject<UP01_PlainObject>();
			TestNotNull(TEXT("【UObject】NewObject 不应为空"), Obj);
			TestTrue(TEXT("【UObject】IsValid"), IsValid(Obj));
		});

		It(TEXT("P02 CDO 与实例不同 ；CDO 写入会影响新实例初始值"), [this]()
		{
			UP02_CDODemoObject* CDO = GetMutableDefault<UP02_CDODemoObject>();
			UP02_CDODemoObject* Inst = NewObject<UP02_CDODemoObject>();
			TestTrue(TEXT("CDO 与实例为不同对象"), CDO != Inst);
			TestTrue(TEXT("CDO 带 RF_ClassDefaultObject 标记"),
				CDO->HasAnyFlags(RF_ClassDefaultObject));

			CDO->TestValue = 42;
			UP02_CDODemoObject* NewInst = NewObject<UP02_CDODemoObject>();
			TestEqual(TEXT("新实例继承 CDO 值"), NewInst->TestValue, 42);
			CDO->TestValue = 10;   // 还原
		});

		It(TEXT("P03 反射可遍历 UPROPERTY"), [this]()
		{
			AP03_ReflectionDemoActor* Actor = NewObject<AP03_ReflectionDemoActor>();
			TArray<FString> Names;
			const int32 Count = Actor->CountAndDumpProperties(Names);
			TestTrue(TEXT("属性数量 > 0"), Count > 0);
			TestTrue(TEXT("应包含 IntField"), Names.Contains(TEXT("IntField")));
		});

		It(TEXT("P03 FindPropertyByName"), [this]()
		{
			AP03_ReflectionDemoActor* Actor = NewObject<AP03_ReflectionDemoActor>();
			FString TypeName;
			TestTrue(TEXT("找到 IntField"),
				Actor->FindPropertyTypeByName(TEXT("IntField"), TypeName));
			TestTrue(TEXT("类型应含 int"), TypeName.Contains(TEXT("int")));
		});

		It(TEXT("P04 Abstract 类不可直接实例化"), [this]()
		{
			TestTrue(TEXT("UP04_AbstractBase 标记 Abstract"),
				UP04_AbstractBase::StaticClass()->HasAnyClassFlags(CLASS_Abstract));
			TestFalse(TEXT("UP04_Blueprintable 不该是 Abstract"),
				UP04_Blueprintable::StaticClass()->HasAnyClassFlags(CLASS_Abstract));
		});

		It(TEXT("P05 J_Replicated 在 RepProps 中"), [this]()
		{
			AP05_PropertySpecifierActor* Actor = NewObject<AP05_PropertySpecifierActor>();
			TArray<FLifetimeProperty> Props;
			Actor->GetLifetimeReplicatedProps(Props);
			FProperty* P = AP05_PropertySpecifierActor::StaticClass()
				->FindPropertyByName(TEXT("J_Replicated"));
			TestNotNull(TEXT("反射可达"), P);
			bool bFound = false;
			for (const FLifetimeProperty& L : Props)
			{
				if (P && L.RepIndex == P->RepIndex) { bFound = true; break; }
			}
			TestTrue(TEXT("J_Replicated 已注册到 RepProps"), bFound);
		});

		It(TEXT("P06 ServerSendInput 反射 + 标记正确"), [this]()
		{
			UFunction* Fn = AP06_FunctionSpecifierPawn::StaticClass()
				->FindFunctionByName(TEXT("ServerSendInput"));
			TestNotNull(TEXT("反射可达"), Fn);
			if (Fn)
			{
				TestTrue(TEXT("FUNC_NetServer"),
					EnumHasAllFlags(Fn->FunctionFlags, FUNC_Net | FUNC_NetServer));
				TestTrue(TEXT("FUNC_NetReliable"),
					EnumHasAllFlags(Fn->FunctionFlags, FUNC_NetReliable));
				TestTrue(TEXT("FUNC_NetValidate"),
					EnumHasAllFlags(Fn->FunctionFlags, FUNC_NetValidate));
			}
		});

		It(TEXT("P07 USTRUCT 拷贝独立"), [this]()
		{
			FP07_DamageInfo A;
			A.Amount = 50.f;
			FP07_DamageInfo B = A;   // 值拷贝
			B.Amount = 100.f;
			TestEqual(TEXT("A 不受 B 影响"), A.Amount, 50.f);
			TestEqual(TEXT("B 改成 100"), B.Amount, 100.f);
		});

		It(TEXT("P08 UENUM 反射可达 + 是 uint8"), [this]()
		{
			TestEqual(TEXT("ET 必须 uint8"),
				static_cast<int32>(sizeof(EP08_WeaponType)), 1);
			UEnum* E = StaticEnum<EP08_WeaponType>();
			TestNotNull(TEXT("StaticEnum 有效"), E);
		});

		It(TEXT("P09 接口 Implements + Cast"), [this]()
		{
			AP09_DamageableActor* Actor = NewObject<AP09_DamageableActor>();
			TestTrue(TEXT("Implements<UDamageable>"),
				Actor->Implements<UP09_Damageable>());
			IP09_Damageable* IFace = Cast<IP09_Damageable>(Actor);
			TestNotNull(TEXT("Cast 应成功"), IFace);

			// 走 Execute_ 静态分发 ：BP 与 C++ 实现都能命中
			const float Remain = IP09_Damageable::Execute_TakeDamage(Actor, 30.f);
			TestEqual(TEXT("Health 100 - 30 = 70"), Remain, 70.f);
		});

		It(TEXT("P10 BP 函数库 SquareInt"), [this]()
		{
			TestEqual(TEXT("Square(7)=49"),
				UP10_BlueprintLibrary::SquareInt(7), 49);
		});
	});

	// ───────────────────── B 章节 ─────────────────────
	Describe(TEXT("B_指针体系"), [this]()
	{
		It(TEXT("P14 TSharedPtr 引用计数"), [this]()
		{
			TSharedPtr<FP14_SmartPointerData> A = MakeShared<FP14_SmartPointerData>(1);
			TSharedPtr<FP14_SmartPointerData> B = A;
			TestEqual(TEXT("两者指向同一对象"), A.Get(), B.Get());
			A.Reset();
			TestTrue(TEXT("A Reset 后 B 仍有效"), B.IsValid());
			TestEqual(TEXT("Value 仍为 1"), B->GetValue(), 1);
		});

		It(TEXT("P14 TUniquePtr 不可拷贝可 Move"), [this]()
		{
			TUniquePtr<FP14_SmartPointerData> U = MakeUnique<FP14_SmartPointerData>(99);
			TestTrue(TEXT("U 持有对象"), U.IsValid());
			TUniquePtr<FP14_SmartPointerData> Moved = MoveTemp(U);
			TestFalse(TEXT("U 已被 move 走"), U.IsValid());
			TestEqual(TEXT("Moved->Value=99"), Moved->GetValue(), 99);
		});

		It(TEXT("P16 FGCObject 能 Track UObject"), [this]()
		{
			FP16_FGCObjectManager Mgr;
			UP01_PlainObject* Obj = NewObject<UP01_PlainObject>();
			Mgr.Track(Obj);
			TestEqual(TEXT("Tracked 数 = 1"), Mgr.GetTrackedNum(), 1);
			TestNotNull(TEXT("能取出"), Mgr.GetTrackedAt(0));
		});
	});

	// ───────────────────── C 章节 ─────────────────────
	Describe(TEXT("C_委托与事件"), [this]()
	{
		It(TEXT("P17 单播 ExecuteIfBound"), [this]()
		{
			AP17_20_DelegateShowcase* Actor = NewObject<AP17_20_DelegateShowcase>();
			float Captured = 0.f;
			Actor->OnDamage.BindLambda([&](float V) { Captured = V; });
			Actor->TriggerDamage(42.f);
			TestEqual(TEXT("OnDamage 收到 42"), Captured, 42.f);
		});

		It(TEXT("P18 多播 Broadcast 多个绑定"), [this]()
		{
			AP17_20_DelegateShowcase* Actor = NewObject<AP17_20_DelegateShowcase>();
			int32 Count = 0;
			Actor->OnHealthChanged.AddLambda([&](float) { Count++; });
			Actor->OnHealthChanged.AddLambda([&](float) { Count++; });
			Actor->BroadcastHealth(50.f);
			TestEqual(TEXT("两个绑定各触发一次"), Count, 2);
		});

		It(TEXT("P19 Delegate Handle Remove"), [this]()
		{
			AP17_20_DelegateShowcase* Actor = NewObject<AP17_20_DelegateShowcase>();
			int32 Count = 0;
			FDelegateHandle H = Actor->OnHealthChanged.AddLambda([&](float) { Count++; });
			Actor->BroadcastHealth(1.f);
			TestEqual(TEXT("Remove 前应触发"), Count, 1);
			Actor->OnHealthChanged.Remove(H);
			Actor->BroadcastHealth(1.f);
			TestEqual(TEXT("Remove 后不触发"), Count, 1);
		});

		It(TEXT("P20 RetVal 委托"), [this]()
		{
			AP17_20_DelegateShowcase* Actor = NewObject<AP17_20_DelegateShowcase>();
			Actor->CanActivateCheck.BindLambda([](int32 ID) { return ID == 1; });
			TestTrue(TEXT("ID=1 -> true"), Actor->CanActivateCheck.Execute(1));
			TestFalse(TEXT("ID=2 -> false"), Actor->CanActivateCheck.Execute(2));
		});
	});

	// ───────────────────── D 章节 ─────────────────────
	Describe(TEXT("D_容器与字符串"), [this]()
	{
		It(TEXT("P21 TArray 综合"), [this]()
		{
			TArray<int32> Arr = { 3, 1, 4, 1, 5 };
			Arr.Sort();
			TestEqual(TEXT("Sort 后第一个 = 1"), Arr[0], 1);
			int32* F = Arr.FindByPredicate([](int32 V) { return V == 4; });
			TestNotNull(TEXT("找到 4"), F);
			const int32 Removed = Arr.RemoveAll([](int32 V) { return V == 1; });
			TestEqual(TEXT("移除 2 个 1"), Removed, 2);
		});

		It(TEXT("P22 TMap FindOrAdd"), [this]()
		{
			TMap<FName, int32> M;
			M.Add(TEXT("A"), 100);
			int32& Ref = M.FindOrAdd(TEXT("B"));
			Ref = 200;
			TestEqual(TEXT("A=100"), *M.Find(TEXT("A")), 100);
			TestEqual(TEXT("B=200"), *M.Find(TEXT("B")), 200);
		});

		It(TEXT("P23 FName 比较 O(1) 同字符串相等"), [this]()
		{
			FName A(TEXT("Test"));
			FName B(TEXT("Test"));
			TestTrue(TEXT("FName 相等"), A == B);
		});

		It(TEXT("P24 TOptional / TTuple"), [this]()
		{
			TOptional<float> Maybe;
			TestFalse(TEXT("初始无值"), Maybe.IsSet());
			Maybe = 3.14f;
			TestTrue(TEXT("赋值后有值"), Maybe.IsSet());

			TTuple<FString, int32> T = MakeTuple(FString(TEXT("A")), 1);
			TestEqual(TEXT("Tuple 第 0 元素"), T.Get<0>(), FString(TEXT("A")));
			TestEqual(TEXT("Tuple 第 1 元素"), T.Get<1>(), 1);
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
