#pragma once

#include "CoreMinimal.h"

/**
 * Helpers class to provide utility functions.
 */
class MULTIPLAYERLEARN_API Helpers
{
public:
    Helpers();
    ~Helpers();

    static void PrintString(const FString& str)
    {
        if (GEngine)
        {
            FString formattedMessage;

            // Check if running in editor or standalone
            if (GIsEditor) // True if running in the Unreal Editor
            {
                // Display client ID in editor
                formattedMessage = FString::Printf(TEXT("[Client %d] %s"), UE::GetPlayInEditorID(), *str);
            }
            else
            {
                // Check NetMode for runtime environment
                if (GWorld)
                {
                    ENetMode NetMode = GWorld->GetNetMode();
                    switch (NetMode)
                    {
                    case NM_Client:
                        formattedMessage = FString::Printf(TEXT("[C] %s"), *str);
                        break;
                    case NM_ListenServer:
                        formattedMessage = FString::Printf(TEXT("[LS] %s"), *str);
                        break;
                    case NM_DedicatedServer:
                        formattedMessage = FString::Printf(TEXT("[DS] %s"), *str);
                        break;
                    case NM_Standalone:
                    default:
                        formattedMessage = FString::Printf(TEXT("[Standalone] %s"), *str);
                        break;
                    }
                }else
                {
                    formattedMessage = FString::Printf(TEXT("[Unknown] %s"), *str);
                }
                
            }

            // Print to screen and log
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, formattedMessage);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *formattedMessage);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GEngine is null when trying to print: %s"), *str);
        }
    }
};
