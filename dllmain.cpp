// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

#include "PluginSetup.h"
#include "LH/Assets.h"
#include "LH/LHSprites.h"
#include "LH/LHCore.h" // mandatory core functions
#include "LH/Config.h" // ini config
#include "LH/CallbackCore.h"

// Plugin functionality
#include <fstream>
#include <iterator>
#include <format>
#include "discord_rpc.h"
#define _CRT_SECURE_NO_WARNINGS


std::string APP_ID = "1117388622975471739";

static void handleDiscordReady(const DiscordUser* connectedUser)
{
    Misc::Print(std::format("Discord: connected to user {}#{}",
        connectedUser->username,
        connectedUser->discriminator), CLR_BLUE);
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    Misc::Print(std::format("Discord: disconnected ({}: {})", errcode, message));
}


int CodeExecuteCallback(YYTKCodeEvent* codeEvent, void* rawContext)
{
    CCode* codeObj = std::get<CCode*>(codeEvent->Arguments());
    CInstance* selfInst = std::get<0>(codeEvent->Arguments());
    CInstance* otherInst = std::get<1>(codeEvent->Arguments());
    // If we have invalid data???
    if (!codeObj)
        return YYTK_INVALIDARG;

    if (!codeObj->i_pName)
        return YYTK_INVALIDARG;   

    auto* context = static_cast<CallbackCoreAttributes*>(rawContext);

    // alloc presence
    DiscordRichPresence presence{};
    bool update = false;
    
    /*
        Check for room events
        room names:
            rm_titles - idk
            rm_creater_dialog - idk
            rm_priamidka - Honestly idk
            rm_load  - Behind the scenes loading?
            rm_intro - intro cutscene
            rm_camp  - the camp
            rm_tutor - the tutorial
            rm_game - the game room
            rm_cutscenes - probably for cutscenes
            rm_music_maker - Music maker
    */

    if (0 == strcmp(codeObj->i_pName, "gml_Room_rm_intro_Create"))
    {
        presence.state = "Introduction";
        presence.details = "Cutscene";
        update = true;
    }
    else
    if (0 == strcmp(codeObj->i_pName, "gml_Room_rm_camp_Create"))
    {
        presence.state = "Playing";
        presence.details = "In Camp";    
        update = true;

    }
    else
    if(0 == strcmp(codeObj->i_pName, "gml_Room_rm_game_Create"))
    {
        presence.state = "Playing";
        presence.details = "On Expedition";
        update = true;
    }
    else
    if (0 == strcmp(codeObj->i_pName, "gml_Room_rm_music_maker_Create"))
    {
        presence.state = "Music Maker";
        presence.details = "Vibing";
        update = true;
    }


    if (update)
    {
        presence.largeImageKey = "loop-hero-new-key-art-logo";
        presence.largeImageText = "Loop Hero modded";
        Discord_UpdatePresence(&presence);
        update = false;
    }
   
    return YYTK_OK;
}


void InstallPatches() // Register Pre and Post patches here
{
    if (LHCore::pInstallPostPatch != nullptr && LHCore::pInstallPrePatch != nullptr)
    {
        LHCore::pInstallPostPatch(CodeExecuteCallback); // Method will run after CodeExecute
        Misc::Print("Installed patch method(s)", CLR_AQUA);
    }

    // Config things
    if (Filesys::FileExists(cfgFilename))
    {
        if (Config::KeySectionExists(cfgFilename, SectionName, KeyName)) {
            APP_ID = Config::ReadIniValue(cfgFilename, SectionName, KeyName, APP_ID);
        }
        else
        {
            Config::WriteIniValue(cfgFilename, SectionName, KeyName, APP_ID);
        }
    }

    // Discord things
    { 
        DiscordEventHandlers handlers{};
        
        handlers.ready = handleDiscordReady;
        handlers.disconnected = handleDiscordDisconnected;

        Discord_Initialize(APP_ID.c_str(), &handlers, 1, NULL);

        // Set to default presence
        DiscordRichPresence presence{};
        
        presence.details = "Main Menu";
        presence.state = "";
        presence.largeImageKey = "loop-hero-new-key-art-logo";
        presence.largeImageText = "Loop Hero modded";
        Discord_UpdatePresence(&presence);
    }
}

DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    LHCore::CoreReadyPack* pack = new LHCore::CoreReadyPack(PluginObject, InstallPatches); // InstallPatches will be ran as soon as CallbackCore is ready.
    CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, (LPVOID)pack, 0, NULL)); // Wait for LHCC
    return YYTK_OK;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

