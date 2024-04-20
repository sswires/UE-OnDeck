// Copyright Stephen Swires. All Rights Reserved.

#include "SteamBigPictureTextField.h"
#include "OnDeckLogChannels.h"

#if ONDECK_STEAM_API_AVAILABLE
	#include "steam/isteamutils.h"
#endif

FSteamBigPictureTextField::FSteamBigPictureTextField()
#if ONDECK_STEAM_API_AVAILABLE
	: GamepadTextInputDismissed(this, &FSteamBigPictureTextField::OnGamepadTextInputDismissed)
#endif
{
}

void FSteamBigPictureTextField::ShowVirtualKeyboard(bool bShow, int32 UserIndex, TSharedPtr<IVirtualKeyboardEntry> TextEntryWidget)
{
#if ONDECK_STEAM_API_AVAILABLE
	const bool bMultiLineEntry = TextEntryWidget->IsMultilineEntry();
	const bool bPasswordEntry = TextEntryWidget->GetVirtualKeyboardType() == Keyboard_Password;
	
	const auto HintTextChar = StringCast<UTF8CHAR>(*TextEntryWidget->GetHintText().ToString());
	const auto PlaceholderTextChar = StringCast<UTF8CHAR>(*TextEntryWidget->GetText().ToString());
	
	const EGamepadTextInputMode TextInputMode = bPasswordEntry ?
		EGamepadTextInputMode::k_EGamepadTextInputModePassword :
		EGamepadTextInputMode::k_EGamepadTextInputModeNormal;
	
	const EGamepadTextInputLineMode LineInputMode = bMultiLineEntry ?
		EGamepadTextInputLineMode::k_EGamepadTextInputLineModeSingleLine :
		EGamepadTextInputLineMode::k_EGamepadTextInputLineModeMultipleLines;

	if (SteamUtils()->ShowGamepadTextInput(TextInputMode,
                                           LineInputMode,
                                           reinterpret_cast<const char*>(HintTextChar.Get()),
                                           0,
                                           reinterpret_cast<const char*>(PlaceholderTextChar.Get())
	))
	{
		UE_LOGFMT(LogOnDeck, Verbose, "Showing virtual keyboard.");
	
		// virtual keyboard shown, so store the widget that requested it
		CurrentTextEntryWidget = TextEntryWidget;
	}
	else
	{
		UE_LOGFMT(LogOnDeck, Warning, "Failed to show virtual keyboard. Overlay might be disabled.");
	}
#endif
}

bool FSteamBigPictureTextField::AllowMoveCursor()
{
	return true;
}

#if ONDECK_STEAM_API_AVAILABLE
void FSteamBigPictureTextField::OnGamepadTextInputDismissed(GamepadTextInputDismissed_t* Param)
{
	const bool bSubmitted = Param->m_bSubmitted;
	const uint32 TextLength = Param->m_unSubmittedText;

	// delegate back to the game thread to handle setting text, this is going to be called from the online thread
	AsyncTask(ENamedThreads::GameThread, [this, bSubmitted, TextLength]()
	{
		if (CurrentTextEntryWidget.IsValid())
		{
			if (bSubmitted)
			{
				// get text entered from steam API
				const uint32 BufferLength = TextLength + 1;
				TArray<char, TInlineAllocator<64>> EnteredText;
				EnteredText.SetNumZeroed(BufferLength);

				SteamUtils()->GetEnteredGamepadTextInput(EnteredText.GetData(), BufferLength);

				// convert to FString/FText
				const FString TextString = UTF8_TO_TCHAR(EnteredText.GetData());
				const FText Text = FText::FromString(TextString);
				
				// submit text to widget
				CurrentTextEntryWidget->SetTextFromVirtualKeyboard(Text, ETextEntryType::TextEntryAccepted);
			}
			else
			{
				CurrentTextEntryWidget->SetTextFromVirtualKeyboard(CurrentTextEntryWidget->GetText(), ETextEntryType::TextEntryCanceled);		
			}
	
			CurrentTextEntryWidget = nullptr;
		}
	});
}
#endif
