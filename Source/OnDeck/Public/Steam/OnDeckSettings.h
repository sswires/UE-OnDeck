// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "OnDeckSettings.generated.h"

/**
 * Settings for OnDeck plugin.
 */
UCLASS(Config = Engine, DefaultConfig, meta = (DisplayName = "Steam Deck Settings (OnDeck)"))
class ONDECK_API UOnDeckSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	// If detected, enable the device profile for Steam Deck.
	UPROPERTY(Config, EditAnywhere)
	bool bEnableDeviceProfile = false;

	// If custom device profile is enabled, this is the name of the device profile to use.
	UPROPERTY(Config, EditAnywhere, meta = (EditCondition = "bEnableDeviceProfile"))
	FString DeviceProfileName = TEXT("SteamDeck");
	
	// Enable virtual keyboard support if Big Picture mode (including on Steam Deck) is detected.
	UPROPERTY(Config, EditAnywhere)
	bool bEnableVirtualKeyboard = true;
	
};
