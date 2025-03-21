// ClickableObject.cpp

#include "ClickableObject.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UClickableObject::UClickableObject()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 默认启用状态
	bEnabled = true;

	// 默认参数名称与数值（可在编辑器中调整）
	HoverParameterName = FName("Highlight");
	HoverValue = 1.0f;
	NormalValue = 0.0f;

	DynMaterial = nullptr;
}

void UClickableObject::BeginPlay()
{
	Super::BeginPlay();

	// 如果在编辑器中指定了目标 MeshComponent，则生成动态材质实例
	if (TargetMeshComponent)
	{
		// 从第 0 个材质槽创建动态材质实例
		DynMaterial = TargetMeshComponent->CreateDynamicMaterialInstance(0);
		if (DynMaterial)
		{
			// 初始化参数为 NormalValue
			DynMaterial->SetScalarParameterValue(HoverParameterName, NormalValue);
		}

		// 开启 Custom Depth 渲染（确保目标 MeshComponent 支持此功能）
		TargetMeshComponent->SetRenderCustomDepth(true);

		// 绑定鼠标悬停事件
		TargetMeshComponent->OnBeginCursorOver.AddDynamic(this, &UClickableObject::OnBeginCursorOver);
		TargetMeshComponent->OnEndCursorOver.AddDynamic(this, &UClickableObject::OnEndCursorOver);
	}
}

void UClickableObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 每帧根据 bEnabled 状态更新 Custom Depth Stencil Value
	UpdateCustomDepth();
}

void UClickableObject::UpdateCustomDepth()
{
	if (TargetMeshComponent)
	{
		// 如果启用，Stencil Value 设为 1；否则设为 0
		int32 StencilValue = bEnabled ? 1 : 0;
		TargetMeshComponent->SetCustomDepthStencilValue(StencilValue);
	}
}

void UClickableObject::OnBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	// 鼠标悬停时修改材质参数为 HoverValue
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue(HoverParameterName, HoverValue);
	}
}

void UClickableObject::OnEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	// 鼠标离开时还原材质参数为 NormalValue
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue(HoverParameterName, NormalValue);
	}
}
