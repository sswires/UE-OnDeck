// Copyright Stephen Swires. All Rights Reserved.

#include "OnDeck.h"
#include "SteamSharedModule.h"
#include "Steam/OnDeckSettings.h"
#include "OnDeckLogChannels.h"
#include "DeviceProfiles/DeviceProfileManager.h"

#if ONDECK_STEAM_API_AVAILABLE
	#include "steam/steam_api.h"
	#include "steam/isteamutils.h"
	#include "Slate/SteamBigPictureTextField.h"
#endif

#if !UE_BUILD_SHIPPING
namespace OnDeckConsoleVariables
{
	TAutoConsoleVariable CvarFakeRunningOnSteamDeck(
		TEXT("OnDeck.FakeRunningOnSteamDeck"),
		false,
		TEXT("Fake running on Steam Deck for testing purposes."),
		ECVF_Default
	);

	TAutoConsoleVariable CvarFakeRunningInSteamBigPicture(
		TEXT("OnDeck.FakeRunningInSteamBigPicture"),
		false,
		TEXT("Fake running in Steam Big Picture mode for testing purposes."),
		ECVF_Default
	);
}
#endif

void FOnDeckModule::StartupModule()
{
#if ONDECK_STEAM_API_AVAILABLE
	// Get Steam API through SteamShared module
	if (!FSteamSharedModule::IsAvailable() || !FSteamSharedModule::Get().AreSteamDllsLoaded() || !SteamUtils())
	{
		// Steam API isn't available
		return;
	}

	// cache values of Steam Deck and Big Picture mode status
	bIsRunningOnSteamDeck = SteamUtils()->IsSteamRunningOnSteamDeck();
	bIsRunningInSteamBigPicture = SteamUtils()->IsSteamInBigPictureMode();

	const UOnDeckSettings* OnDeckSettings = GetDefault<UOnDeckSettings>();
	
	// check to see if we should enable the Steam Deck device profile
	if (OnDeckSettings->bEnableDeviceProfile)
	{
		// we haven opted into the device profile functionality, check if it should be enabled
		CheckDeviceProfile(OnDeckSettings->DeviceProfileName);
	}

	// set the custom virtual keyboard handler if enabled
	if (OnDeckSettings->bEnableVirtualKeyboard && bIsRunningInSteamBigPicture)
	{
		FSlateApplication::Get().OverridePlatformTextField(MakeUnique<FSteamBigPictureTextField>());
		UE_LOGFMT(LogOnDeck, Log, "Overriding virtual keyboard for Steam Big Picture mode.");
	}
#endif
}

void FOnDeckModule::ShutdownModule()
{
}

bool FOnDeckModule::IsRunningOnSteamDeck() const
{
#if !UE_BUILD_SHIPPING
	if (OnDeckConsoleVariables::CvarFakeRunningOnSteamDeck.GetValueOnGameThread())
	{
		return true;
	}
#endif

	return bIsRunningOnSteamDeck;
}

bool FOnDeckModule::IsRunningInSteamBigPicture() const
{
#if !UE_BUILD_SHIPPING
	if (OnDeckConsoleVariables::CvarFakeRunningInSteamBigPicture.GetValueOnGameThread())
	{
		return true;
	}

	if (IsRunningOnSteamDeck())
	{
		// Steam Deck is always in Big Picture mode so if we're "emulating" then we should return true here as well
		return true;
	}
#endif
	
	return bIsRunningInSteamBigPicture;
}

void FOnDeckModule::CheckDeviceProfile(const FString& InDeviceProfileName)
{
	if (!bIsRunningOnSteamDeck)
	{
		// not running on a Steam Deck, ignore
		UE_LOGFMT(LogOnDeck, Verbose, "Not running on Steam Deck, skipping device profile.");
		return;
	}

	const FString DeviceProfileName = InDeviceProfileName.IsEmpty() ? TEXT("SteamDeck") : InDeviceProfileName;
	UDeviceProfileManager& DeviceProfileManager = UDeviceProfileManager::Get();
	UDeviceProfile* SteamDeckProfile = DeviceProfileManager.FindProfile(DeviceProfileName);

	if (!SteamDeckProfile)
	{
		// Steam Deck profile already exists, no need to create it
		UE_LOGFMT(LogOnDeck, Error, "Steam Deck device profile \"{0}\" does not exist.", DeviceProfileName);
		return;
	}
	
	DeviceProfileManager.SetOverrideDeviceProfile(SteamDeckProfile);
	UE_LOGFMT(LogOnDeck, Log, "Steam Deck device profile \"{0}\" applied.", DeviceProfileName);
}

IMPLEMENT_MODULE(FOnDeckModule, OnDeck)
