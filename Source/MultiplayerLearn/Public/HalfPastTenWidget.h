// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "HalfPastTenLogic.h"
#include "HalfPastTenWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERLEARN_API UHalfPastTenWidget : public UUserWidget
{
	GENERATED_BODY()
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HalfPastTenWidget_Ref")
    UTextBlock* TxtHighestBid;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HalfPastTenWidget_Ref")
    UTextBlock* TxtMyBid;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HalfPastTenWidget_Auto")
    AHalfPastTenLogic* HalfPastTenLogic;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HalfPastTenWidget_Auto")
    AHalfPastTenPlayer* OwningPlayer;
    
    //Buttons
    UFUNCTION(BlueprintCallable, Category = "HalfPastTenWidget")
    void OnBidButtonClicked();
    
    UFUNCTION(BlueprintCallable, Category = "HalfPastTenWidget")
    void OnWaiveButtonClicked();
    
    UFUNCTION(BlueprintCallable, Category = "HalfPastTenWidget")
    void OnRaiseButtonClicked(int raiseAmount);
    
protected:

    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    virtual bool Initialize() override;
};
