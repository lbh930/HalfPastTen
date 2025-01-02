#include "HalfPastTenWidget.h"
#include "Helpers.h"
#include "Engine/World.h"

void UHalfPastTenWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    HalfPastTenLogic = Cast<AHalfPastTenLogic>(GetWorld()->GetGameState());
    
    //Get Current Owning Player as HalfPastTenPlayer
    OwningPlayer = Cast<AHalfPastTenPlayer>(GetOwningPlayerPawn());
        
    UE_LOG(LogTemp, Log, TEXT("HalfPastTenWidget Constructed!"));
}

void UHalfPastTenWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);
    
    if (OwningPlayer == nullptr){
        OwningPlayer = Cast<AHalfPastTenPlayer>(GetOwningPlayerPawn());
    }
    
    if (HalfPastTenLogic == nullptr){
        Helpers::PrintString("UHalfPastTenWidget::NativeTick() - HalfPastTenLogic is nullptr");
    }else{
        FString highestBidText = "Highest Bid: ";
        highestBidText += FString::FromInt(HalfPastTenLogic->HighestBid);
        highestBidText += "\n by Player ";
        highestBidText += FString::FromInt(HalfPastTenLogic->HighestBidPlayerId);
        if (TxtHighestBid){
            TxtHighestBid->SetText(FText::FromString(highestBidText));
        }else{
            Helpers::PrintString("UHalfPastTenWidget::NativeTick() - HighestBidTextBlock is nullptr");
        }
        
        int CurrentBid = -1;
        if (OwningPlayer){
            CurrentBid = OwningPlayer->GetCurrentBid();
        }else{
            Helpers::PrintString("UHalfPastTenWidget::NativeTick() - OwningPlayer is nullptr");
        }
        
        if (TxtMyBid){
            TxtMyBid->SetText(FText::FromString(FString::Printf(TEXT("My Bid: %d"), CurrentBid)));
        }else{
            Helpers::PrintString("UHalfPastTenWidget::NativeTick() - MyBidTextBlock is nullptr");
        
        }
    }
}

bool UHalfPastTenWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    return bSuccess;
}

void UHalfPastTenWidget::OnBidButtonClicked(){
    Helpers::PrintString("UHalfPastTenWidget::OnBidButtonClicked()");
    if (OwningPlayer){
        OwningPlayer->ServerBid(OwningPlayer->GetPlayerId(), OwningPlayer->GetCurrentBid());
    }else{
        Helpers::PrintString("UHalfPastTenWidget::OnBidButtonClicked() - OwningPlayer is nullptr");
    }
}

void UHalfPastTenWidget::OnWaiveButtonClicked(){
    if (OwningPlayer){
        OwningPlayer->ServerWaive();
    }else{
        Helpers::PrintString("UHalfPastTenWidget::OnWaiveButtonClicked() - OwningPlayer is nullptr");
    }
}

void UHalfPastTenWidget::OnRaiseButtonClicked(int newBidAdded){
    if (OwningPlayer){
        OwningPlayer->SetCurrentBid(OwningPlayer->GetCurrentBid() + newBidAdded);
    }else{
        Helpers::PrintString("UHalfPastTenWidget::OnRaiseButtonClicked() - OwningPlayer is nullptr");
    }
}

void UHalfPastTenWidget::OnStrikeButtonClicked(){
    if (OwningPlayer){
        OwningPlayer->ServerStrike(OwningPlayer->GetPlayerId(), OwningPlayer->GetCurrentBid());
    }else{
        Helpers::PrintString("UHalfPastTenWidget::OnStrikeButtonClicked() - OwningPlayer is nullptr");
    }
}


