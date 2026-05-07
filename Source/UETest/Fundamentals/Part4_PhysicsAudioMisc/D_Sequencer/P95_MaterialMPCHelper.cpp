#include "P95_MaterialMPCHelper.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/World.h"

UMaterialInstanceDynamic* UP95_MaterialMPCHelper::CreateMIDAndSet(
	UMeshComponent* Mesh, int32 SlotIndex, FName ScalarName, float ScalarValue)
{
	if (!Mesh) { return nullptr; }
	UMaterialInstanceDynamic* MID = Mesh->CreateDynamicMaterialInstance(SlotIndex);
	if (MID)
	{
		MID->SetScalarParameterValue(ScalarName, ScalarValue);
	}
	return MID;
}

void UP95_MaterialMPCHelper::SetMPCScalar(
	const UObject* WorldContext, UMaterialParameterCollection* MPC,
	FName ScalarName, float Value)
{
	if (!WorldContext || !MPC) { return; }
	if (UMaterialParameterCollectionInstance* Inst =
		WorldContext->GetWorld()->GetParameterCollectionInstance(MPC))
	{
		Inst->SetScalarParameterValue(ScalarName, Value);
	}
}

void UP95_MaterialMPCHelper::SetMPCVector(
	const UObject* WorldContext, UMaterialParameterCollection* MPC,
	FName VectorName, FLinearColor Value)
{
	if (!WorldContext || !MPC) { return; }
	if (UMaterialParameterCollectionInstance* Inst =
		WorldContext->GetWorld()->GetParameterCollectionInstance(MPC))
	{
		Inst->SetVectorParameterValue(VectorName, Value);
	}
}
