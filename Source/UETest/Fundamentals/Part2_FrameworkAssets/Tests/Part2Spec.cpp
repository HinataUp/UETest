// Part 2 - 自动化测试
//
// 单测覆盖 ：组件 / Actor 默认配置 ；反射检查 ；数据结构正确性
// 不覆盖 ：完整生命周期（需 PIE）/ DataTable 行查询（需资产）/ 网络复制（需 PIE）

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Net/UnrealNetwork.h"

#include "../A_ActorComponent/P31_LifecycleActor.h"
#include "../A_ActorComponent/P32_ComponentManagementActor.h"
#include "../A_ActorComponent/P34_ComponentLookupActor.h"
#include "../B_Framework/P35_LifecycleGameInstance.h"
#include "../B_Framework/P36_LifecycleGameMode.h"
#include "../B_Framework/P37_LifecyclePlayerController.h"
#include "../B_Framework/P38_39_LifecycleCharacter.h"
#include "../C_Subsystems/P41_AllSubsystems.h"
#include "../C_Subsystems/P42_NativeTagsP2.h"
#include "../C_Subsystems/P43_DamageMessage.h"
#include "../D_DataAssetTable/P44_HeroDefinition.h"
#include "../D_DataAssetTable/P45_WeaponDataTable.h"
#include "../E_AssetLoading/P47_50_AssetLoadingHelper.h"
#include "../F_SaveConfigLocalize/P52_MySaveGame.h"
#include "../F_SaveConfigLocalize/P53_MySettingsConfig.h"
#include "../F_SaveConfigLocalize/P54_LocalizationDemo.h"
#include "../G_ModulesPlugins/P57_CVarCommandDemo.h"
#include "../G_ModulesPlugins/P58_PreprocessorDemo.h"
#include "../H_NetworkingLevel/P59_NetworkBasicsActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

BEGIN_DEFINE_SPEC(
	FUETest_FundamentalsP2_Spec,
	"UETest.Fundamentals.P2_框架与资产",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_FundamentalsP2_Spec)

void FUETest_FundamentalsP2_Spec::Define()
{
	Describe(TEXT("A_Actor与Component"), [this]()
	{
		It(TEXT("P31 构造函数标记 Stage_Ctor"), [this]()
		{
			AP31_LifecycleActor* Actor = NewObject<AP31_LifecycleActor>();
			TestTrue(TEXT("Stage_Ctor 应被设置"),
				(Actor->StageFlags & AP31_LifecycleActor::Stage_Ctor) != 0);
		});

		It(TEXT("P32 RootScene 已创建"), [this]()
		{
			AP32_ComponentManagementActor* Actor = NewObject<AP32_ComponentManagementActor>();
			TestNotNull(TEXT("RootScene 不为空"), Actor->RootScene.Get());
			TestEqual(TEXT("RootScene 即 RootComponent"),
				static_cast<UObject*>(Actor->RootComponent.Get()),
				static_cast<UObject*>(Actor->RootScene.Get()));
		});

		It(TEXT("P34 三个 Mesh 默认带 Tag"), [this]()
		{
			AP34_ComponentLookupActor* Actor = NewObject<AP34_ComponentLookupActor>();
			TestTrue(TEXT("WeaponMesh 带 Weapon Tag"),
				Actor->WeaponMesh->ComponentHasTag(TEXT("Weapon")));
			TestTrue(TEXT("ShieldMesh 带 Shield Tag"),
				Actor->ShieldMesh->ComponentHasTag(TEXT("Shield")));
			TestTrue(TEXT("BodyMesh 带 Body Tag"),
				Actor->BodyMesh->ComponentHasTag(TEXT("Body")));
		});

		It(TEXT("P34 FindMeshByTag 能正确返回"), [this]()
		{
			AP34_ComponentLookupActor* Actor = NewObject<AP34_ComponentLookupActor>();
			UStaticMeshComponent* Found = Actor->FindMeshByTag(TEXT("Shield"));
			TestEqual(TEXT("找到 ShieldMesh"),
				static_cast<UObject*>(Found),
				static_cast<UObject*>(Actor->ShieldMesh.Get()));
		});
	});

	Describe(TEXT("B_Gameplay框架"), [this]()
	{
		It(TEXT("P36 GameMode.GameStateClass = AP36_GameState"), [this]()
		{
			AP36_GameMode* GM = NewObject<AP36_GameMode>();
			TestEqual(TEXT("GameStateClass 已绑定"),
				GM->GameStateClass.Get(), AP36_GameState::StaticClass());
		});

		It(TEXT("P36 GameState bReplicates"), [this]()
		{
			AP36_GameState* GS = NewObject<AP36_GameState>();
			TestTrue(TEXT("bReplicates"), GS->GetIsReplicated());

			TArray<FLifetimeProperty> Props;
			GS->GetLifetimeReplicatedProps(Props);
			FProperty* P = AP36_GameState::StaticClass()->FindPropertyByName(TEXT("MatchTimer"));
			TestNotNull(TEXT("反射可达"), P);
			bool bFound = false;
			for (const FLifetimeProperty& L : Props)
			{
				if (P && L.RepIndex == P->RepIndex) { bFound = true; break; }
			}
			TestTrue(TEXT("MatchTimer 在 RepProps 中"), bFound);
		});

		It(TEXT("P37 PlayerState bReplicates + PlayerScore"), [this]()
		{
			AP37_PlayerState* PS = NewObject<AP37_PlayerState>();
			TestTrue(TEXT("bReplicates"), PS->GetIsReplicated());
			TestEqual(TEXT("默认 PlayerScore=0"), PS->PlayerScore, 0);
		});

		It(TEXT("P38 Character 有 IA 字段"), [this]()
		{
			// 反射检查 ：编辑器配置点是否暴露
			FProperty* P = AP38_LifecycleCharacter::StaticClass()
				->FindPropertyByName(TEXT("IA_Move"));
			TestNotNull(TEXT("IA_Move 字段反射可达"), P);
		});
	});

	Describe(TEXT("C_Subsystems与Tag"), [this]()
	{
		It(TEXT("P41 四个 Subsystem 类反射可达"), [this]()
		{
			TestNotNull(TEXT("Engine"), UP41_EngineSubsystem::StaticClass());
			TestNotNull(TEXT("GameInstance"), UP41_GameInstanceSubsystem::StaticClass());
			TestNotNull(TEXT("World"), UP41_WorldSubsystem::StaticClass());
			TestNotNull(TEXT("LocalPlayer"), UP41_LocalPlayerSubsystem::StaticClass());
		});

		It(TEXT("P42 Tag Native 注册成功"), [this]()
		{
			TestTrue(TEXT("Damage Tag valid"),
				P2_Tags::TAG_Message_Damage_Dealt.IsValid());
			TestTrue(TEXT("Joined Tag valid"),
				P2_Tags::TAG_Message_Player_Joined.IsValid());
		});

		It(TEXT("P43 DamageMessage struct 字段"), [this]()
		{
			FP43_DamageMessage Msg;
			Msg.Amount = 50.f;
			TestEqual(TEXT("Amount=50"), Msg.Amount, 50.f);
		});
	});

	Describe(TEXT("D_数据资产"), [this]()
	{
		It(TEXT("P44 PrimaryAssetId 类型正确"), [this]()
		{
			UP44_HeroDefinition* Asset = NewObject<UP44_HeroDefinition>();
			Asset->Rename(TEXT("DA_Test_Hero"));
			const FPrimaryAssetId Id = Asset->GetPrimaryAssetId();
			TestEqual(TEXT("Type = Hero"),
				Id.PrimaryAssetType.GetName().ToString(), FString(TEXT("Hero")));
		});

		It(TEXT("P45 RowStruct 默认值"), [this]()
		{
			FP45_WeaponData Row;
			TestEqual(TEXT("Damage 默认 0"), Row.Damage, 0.f);
			TestEqual(TEXT("AttackSpeed 默认 1"), Row.AttackSpeed, 1.f);
		});
	});

	Describe(TEXT("E_资源加载"), [this]()
	{
		It(TEXT("P47 SyncLoad 空路径返回 nullptr"), [this]()
		{
			UObject* L = UP47_50_AssetLoadingComponent::SyncLoadByPath(FSoftObjectPath());
			TestNull(TEXT("空路径无加载"), L);
		});
	});

	Describe(TEXT("F_存档配置本地化"), [this]()
	{
		It(TEXT("P52 SaveGame 默认值 + InventoryItem 有 SaveGame 字段"), [this]()
		{
			UP52_MySaveGame* Save = NewObject<UP52_MySaveGame>();
			TestEqual(TEXT("默认 Level=1"), Save->Level, 1);

			UStruct* S = FP52_InventoryItem::StaticStruct();
			TestNotNull(TEXT("ItemId 字段"),
				S->FindPropertyByName(TEXT("ItemId")));
		});

		It(TEXT("P53 Config 类有 Config Specifier"), [this]()
		{
			TestTrue(TEXT("Class 标记 Config"),
				UP53_MySettingsConfig::StaticClass()->HasAnyClassFlags(CLASS_Config));
		});

		It(TEXT("P54 FormatDamageText 含中文模板替换"), [this]()
		{
			const FText T = UP54_LocalizationHelper::FormatDamageText(TEXT("Goblin"), 50.f);
			const FString S = T.ToString();
			TestTrue(TEXT("含 Goblin"), S.Contains(TEXT("Goblin")));
			TestTrue(TEXT("含 50"), S.Contains(TEXT("50")));
		});
	});

	Describe(TEXT("G_工程化"), [this]()
	{
		It(TEXT("P57 Pawn bReplicates"), [this]()
		{
			AP57_CVarCommandPawn* P = NewObject<AP57_CVarCommandPawn>();
			TestTrue(TEXT("bReplicates"), P->GetIsReplicated());
		});

		It(TEXT("P58 GetPlatformName 非空"), [this]()
		{
			FString Name = UP58_PreprocessorHelper::GetPlatformName();
			TestFalse(TEXT("平台名非空"), Name.IsEmpty());
		});
	});

	Describe(TEXT("H_网络与关卡"), [this]()
	{
		It(TEXT("P59 NetworkBasicsActor RPC 标记正确"), [this]()
		{
			UFunction* ServerFn = AP59_NetworkBasicsActor::StaticClass()
				->FindFunctionByName(TEXT("ServerRequestDamage"));
			TestNotNull(TEXT("Server RPC 反射"), ServerFn);
			if (ServerFn)
			{
				TestTrue(TEXT("FUNC_NetServer + Reliable + Validate"),
					EnumHasAllFlags(ServerFn->FunctionFlags,
						FUNC_Net | FUNC_NetServer | FUNC_NetReliable | FUNC_NetValidate));
			}

			UFunction* MultiFn = AP59_NetworkBasicsActor::StaticClass()
				->FindFunctionByName(TEXT("MulticastPlayHitFX"));
			TestNotNull(TEXT("Multicast 反射"), MultiFn);
			if (MultiFn)
			{
				TestTrue(TEXT("FUNC_NetMulticast"),
					EnumHasAllFlags(MultiFn->FunctionFlags, FUNC_Net | FUNC_NetMulticast));
			}
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
