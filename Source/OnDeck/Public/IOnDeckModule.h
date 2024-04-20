// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class IOnDeckModule : public IModuleInterface
{
public:
    virtual bool IsRunningOnSteamDeck() const = 0;
    virtual bool IsRunningInSteamBigPicture() const = 0;

    static bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("OnDeck");
    }

    static IOnDeckModule& Get()
    {
        return FModuleManager::GetModuleChecked<IOnDeckModule>("OnDeck");
    }
};
