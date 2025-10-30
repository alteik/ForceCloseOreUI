//
// Created by alteik on 30.10.2025.
//

#include <libhat/scanner.hpp>
#include <libhat/signature.hpp>
#include "utils/detour.hpp"
#include "sdk/oreui.hpp"

// void OreUI::OreUISystem::_registerToggleObservers()
std::unique_ptr<detour> gDoRegisterDetour = nullptr;
hat::fixed_signature gDoRegisterSig = hat::compile_signature<"40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B F1 4C 89 44 24 ? 48 89 54 24">();

static void registerToggleObservers(
    void* _this,
    void* a2,
    void* a3,
    void* a4,
    void* a5,
    void* a6,
    void* a7,
    void* a8,
    void* a9,
    OreUi* ui,
    void* a11)
{
    auto original = gDoRegisterDetour->getOriginal<&registerToggleObservers>();

    for (auto &[id, config]: ui->mConfigs)
    {
        if (id == "/play")
        {
            config.mFunc1 = []() -> bool { return false; };
            config.mFunc2 = config.mFunc1;
        }
    }

    original(_this, a2, a3, a4, a5, a6, a7, a8, a9, ui, a11);
}

BOOL WINAPI DllMain(HMODULE /* module */, DWORD reason,  LPVOID /* reserved */)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            MH_Initialize();
            hat::scan_result isTrialAddr = hat::find_pattern(gDoRegisterSig, ".text");
            gDoRegisterDetour = std::make_unique<detour>(isTrialAddr.get(), &registerToggleObservers);
            gDoRegisterDetour->enable();

            break;
        }
        case DLL_PROCESS_DETACH:
        {
            gDoRegisterDetour->disable();
            gDoRegisterDetour.reset();
            MH_Uninitialize();

            break;
        }

        default: break;
    }

    return TRUE;
}