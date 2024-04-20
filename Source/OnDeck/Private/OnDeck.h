// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IOnDeckModule.h"

class FOnDeckModule : public IOnDeckModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** IOnDeckModuleInterface implementation */
	virtual bool IsRunningOnSteamDeck() const override;
	virtual bool IsRunningInSteamBigPicture() const override;

private:

	void CheckDeviceProfile(const FString& InDeviceProfileName);

	bool bIsRunningOnSteamDeck = false;
	bool bIsRunningInSteamBigPicture = false;
	
};
