// ClickableObject.h
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ClickableObject.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UPrimitiveComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYERLEARN_API UClickableObject : public USceneComponent
{
	GENERATED_BODY()

public:
	// 构造函数：设置默认值
	UClickableObject();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

public:
	// 每帧调用
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 指定场景中已有的一个 MeshComponent，其上附着了你需要操作的材质。
	 * 在编辑器里将目标物体的 MeshComponent 拖拽至此处即可。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clickable")
	UStaticMeshComponent* TargetMeshComponent;

	/**
	 * @brief 控制是否启用该对象的显示效果，启用时 Custom Depth Stencil Value 为 1，否则为 0。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clickable")
	bool bEnabled;

	/**
	 * @brief 要修改的材质参数名称，例如 "Highlight"。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clickable")
	FName HoverParameterName;

	/**
	 * @brief 鼠标悬停时要设置的参数值。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clickable")
	float HoverValue;

	/**
	 * @brief 鼠标不悬停时的参数值。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clickable")
	float NormalValue;

private:
	/** 从目标材质生成的动态材质实例 */
	UMaterialInstanceDynamic* DynMaterial;

	/** 当鼠标指向目标组件时调用 */
	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	/** 当鼠标离开目标组件时调用 */
	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* TouchedComponent);

	/** 根据 bEnabled 状态更新目标组件的 Custom Depth Stencil Value */
	void UpdateCustomDepth();
};
