// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "Framework/Application/IPlatformTextField.h"

#if ONDECK_STEAM_API_AVAILABLE
	#include "steam/steam_api_common.h"
	#include "steam/isteamutils.h"
#endif

class IVirtualKeyboardEntry;

class FSteamBigPictureTextField : public IPlatformTextField
{
public:
	FSteamBigPictureTextField();
	
	virtual void ShowVirtualKeyboard(bool bShow, int32 UserIndex, TSharedPtr<IVirtualKeyboardEntry> TextEntryWidget);
	virtual bool AllowMoveCursor() override;

private:

#if ONDECK_STEAM_API_AVAILABLE
	STEAM_CALLBACK(FSteamBigPictureTextField, OnGamepadTextInputDismissed, GamepadTextInputDismissed_t, GamepadTextInputDismissed);
#endif

	TSharedPtr<IVirtualKeyboardEntry> CurrentTextEntryWidget;
	
};
