#include "P83_ControlRigHelper.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "../00_Common/FundP3LogMacros.h"

UP83_ControlRigHelper::UP83_ControlRigHelper()
{
	PrimaryComponentTick.bCanEverTick = false;
}

USkeletalMeshComponent* UP83_ControlRigHelper::GetOwnerMesh() const
{
	if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		return Char->GetMesh();
	}
	if (AActor* O = GetOwner())
	{
		return O->FindComponentByClass<USkeletalMeshComponent>();
	}
	return nullptr;
}

void UP83_ControlRigHelper::SetAnimFloat(FName VariableName, float Value)
{
	USkeletalMeshComponent* Mesh = GetOwnerMesh();
	if (!Mesh) { return; }
	UAnimInstance* AnimInst = Mesh->GetAnimInstance();
	if (!AnimInst) { return; }

	// 通过反射设置 AnimInstance 上的 float UPROPERTY
	// AnimBP 中读此字段传给 Control Rig 节点的 Curve / Variable 输入
	if (FProperty* Prop = AnimInst->GetClass()->FindPropertyByName(VariableName))
	{
		if (FFloatProperty* Fp = CastField<FFloatProperty>(Prop))
		{
			Fp->SetFloatingPointPropertyValue(Fp->ContainerPtrToValuePtr<void>(AnimInst), Value);
			P3_LOG("P83: SetAnimFloat %s = %.3f", *VariableName.ToString(), Value);
			return;
		}
	}
	P3_LOG("P83: 未找到 float 变量 %s", *VariableName.ToString());
}

void UP83_ControlRigHelper::SetAnimVector(FName VariableName, FVector Value)
{
	USkeletalMeshComponent* Mesh = GetOwnerMesh();
	if (!Mesh) { return; }
	UAnimInstance* AnimInst = Mesh->GetAnimInstance();
	if (!AnimInst) { return; }

	if (FProperty* Prop = AnimInst->GetClass()->FindPropertyByName(VariableName))
	{
		if (FStructProperty* Sp = CastField<FStructProperty>(Prop))
		{
			if (Sp->Struct == TBaseStructure<FVector>::Get())
			{
				*Sp->ContainerPtrToValuePtr<FVector>(AnimInst) = Value;
				return;
			}
		}
	}
}
