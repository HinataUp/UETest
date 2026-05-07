#include "T07_CustomStencilHelper.h"
#include "Components/PrimitiveComponent.h"
#include "../../00_Common/RenderingLogMacros.h"

void UT07_CustomStencilHelper::EnableXRayOutline(UPrimitiveComponent* Primitive, int32 StencilValue)
{
	if (!Primitive) { return; }

	// 启用额外的 Custom Depth/Stencil Pass
	Primitive->SetRenderCustomDepth(true);
	// Stencil 是 8bit ：0-255 ，不同类别用不同编号
	Primitive->SetCustomDepthStencilValue(FMath::Clamp(StencilValue, 0, 255));
	RT_LOG("Stencil 启用 ：%s = %d", *Primitive->GetName(), StencilValue);
}

void UT07_CustomStencilHelper::DisableXRayOutline(UPrimitiveComponent* Primitive)
{
	if (!Primitive) { return; }
	Primitive->SetRenderCustomDepth(false);
	Primitive->SetCustomDepthStencilValue(0);
}
