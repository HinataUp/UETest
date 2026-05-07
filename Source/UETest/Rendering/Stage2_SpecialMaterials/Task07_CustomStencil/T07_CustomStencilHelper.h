// T07 - X-Ray 透视描边 Custom Stencil 标记 helper
//
// 启用 Custom Depth-Stencil Pass 是项目设置 ；标记某个 Mesh 是 C++ 操作。
// 描边 Post-Process Material 在编辑器中制作（采样 SceneTexture:CustomStencil）。
//
// 本类提供 BP 调用的快捷接口 ：把目标 Mesh 加入或移出"X-Ray 描边集合"。

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "T07_CustomStencilHelper.generated.h"

class UPrimitiveComponent;

UCLASS()
class UETEST_API UT07_CustomStencilHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 让 Mesh 参与 Custom Depth/Stencil Pass ，并设置 stencil 编号
	// PP 材质里通过 if (Stencil == StencilValue) 判断是否描边
	UFUNCTION(BlueprintCallable, Category = "Render|Stencil")
	static void EnableXRayOutline(UPrimitiveComponent* Primitive, int32 StencilValue = 1);

	// 关闭描边
	UFUNCTION(BlueprintCallable, Category = "Render|Stencil")
	static void DisableXRayOutline(UPrimitiveComponent* Primitive);
};
