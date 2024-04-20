// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnDeckFunctionLibrary.generated.h"

/**
 * Blueprint accessible function library to query if we're running on a Steam Deck or we're in big picture mode.
 */
UCLASS()
class ONDECK_API UOnDeckFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Returns true if the game is running on a Steam Deck.
	UFUNCTION(BlueprintPure, Category = "Steam")
	static bool IsRunningOnSteamDeck();

	// Returns true if the game was launched through Steam in Big Picture mode.
	UFUNCTION(BlueprintPure, Category = "Steam")
	static bool IsRunningInSteamBigPicture();
	
};
