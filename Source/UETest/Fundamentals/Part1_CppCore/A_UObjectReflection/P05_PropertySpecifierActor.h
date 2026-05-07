// P05 - UPROPERTY Specifier 完整对比
//
// 编辑器可见性 ：EditAnywhere / EditDefaultsOnly / EditInstanceOnly / VisibleAnywhere
// 蓝图可见性  ：BlueprintReadWrite / BlueprintReadOnly
// 特殊        ：Transient / SaveGame / Replicated / Instanced / Config / meta=(...)
//
// 在编辑器选中此 Actor 实例 / 打开蓝图子类，对照 Details 面板验证哪些字段可编辑

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P05_PropertySpecifierActor.generated.h"

UCLASS(Blueprintable)
class UETEST_API AP05_PropertySpecifierActor : public AActor
{
	GENERATED_BODY()

public:
	AP05_PropertySpecifierActor();

	// 1) 任何地方都能编辑 ：BP CDO + 关卡实例 + 派生 BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P05|Visibility")
	float A_EditAnywhere = 0.f;

	// 2) 只能在 BP CDO（默认值）编辑 ；选中关卡实例时 灰色
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P05|Visibility")
	float B_EditDefaultsOnly = 0.f;

	// 3) 只能在关卡实例编辑 ；BP CDO 中 灰色
	UPROPERTY(EditInstanceOnly, Category = "P05|Visibility")
	float C_EditInstanceOnly = 0.f;

	// 4) 任何地方都只读 ：常用于运行时填值的"展示" 字段
	UPROPERTY(VisibleAnywhere, Category = "P05|Visibility")
	float D_VisibleAnywhere = 0.f;

	// 5) 不参与序列化 ：每次创建从默认值初始化 ；适合纯运行时缓存
	UPROPERTY(Transient)
	float E_Transient = 0.f;

	// 6) SaveGame ：USaveGame 序列化时包含此字段
	UPROPERTY(SaveGame)
	float F_SaveGame = 0.f;

	// 7) meta ClampMin / ClampMax ：编辑器拖滑块时锁定范围
	UPROPERTY(EditAnywhere, Category = "P05|Meta", meta = (ClampMin = "0", ClampMax = "100"))
	float G_Clamped = 50.f;

	// 8) meta UIMin / UIMax ：仅滑块限制 ；输入框可超出
	UPROPERTY(EditAnywhere, Category = "P05|Meta", meta = (UIMin = "0", UIMax = "10"))
	float H_UIRange = 5.f;

	// 9) Instanced ：Editable 子对象（外部可指向，但编辑时是嵌入式拷贝）
	//    适合"组合的小数据对象"
	UPROPERTY(EditAnywhere, Instanced, Category = "P05|Special")
	TObjectPtr<UObject> I_Instanced;

	// 10) Replicated ：参与网络同步（需要 GetLifetimeReplicatedProps）
	UPROPERTY(Replicated)
	float J_Replicated = 0.f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
