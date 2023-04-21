#include <Windows.h>
#include <iostream>

#include "Includes/imgui_hook.h"

#include "Hack.h"

void Main(HMODULE hModule) {
    FILE* pFile = nullptr;
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    Utils::TypewriterPrint("Successfully Loaded OrmBunke!\n\n", 35);

    Utils::TypewriterPrint("Please Report Any Bugs Or Crashing On The GitHub Or Message Me Personally 0xVidde#1337!\n\n", 100);

    Utils::TypewriterPrint("--- Keybinds ---\n", 180);
    Utils::TypewriterPrint("[NUM 0] << Name ESP\n", 180);
    Utils::TypewriterPrint("[NUM 1] << Filled Box ESP\n", 180);
    Utils::TypewriterPrint("[NUM 2] << Box ESP\n", 180);
    Utils::TypewriterPrint("[NUM 3] << Tracer ESP\n", 180);
    Utils::TypewriterPrint("[NUM 4] << Bar ESP\n", 180);
    Utils::TypewriterPrint("[NUM 5] << Head Circle ESP\n", 180);
    Utils::TypewriterPrint("[NUM 6] << Rotation Lines ESP\n", 180);
    Utils::TypewriterPrint("[NUM 7] << FOV Circle\n\n", 180);

    Utils::TypewriterPrint("[NUM 8] << Toggle TriggerBot\n", 180);
    Utils::TypewriterPrint("[NUM 9] << Toggle Aimbot (Left Shift)\n\n", 180);

    Utils::TypewriterPrint("[Right Arrow] << Increase Aimbot FOV\n", 180);
    Utils::TypewriterPrint("[Left Arrow] << Decrease Aimbot FOV\n\n", 180);

    Utils::TypewriterPrint("[END] << Close\n", 180);
    Utils::TypewriterPrint("----------------\n", 180);


    while (!GetAsyncKeyState(VK_END))
    {
        HackMain();

        Sleep(1);
    }

    Utils::TypewriterPrint("Unloading... (You can now close this console)", 250);

    FreeConsole();

    ImGuiHook::Unload();

    FreeLibraryAndExitThread(hModule, 1);
    CloseHandle(hModule);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, 0));
        ImGuiHook::Load(RenderMain);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

