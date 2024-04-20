// Copyright Stephen Swires. All Rights Reserved.

#include "Steam/OnDeckFunctionLibrary.h"
#include "IOnDeckModule.h"

bool UOnDeckFunctionLibrary::IsRunningOnSteamDeck()
{
	return IOnDeckModule::Get().IsRunningOnSteamDeck();
}

bool UOnDeckFunctionLibrary::IsRunningInSteamBigPicture()
{
	return IOnDeckModule::Get().IsRunningInSteamBigPicture();
}
