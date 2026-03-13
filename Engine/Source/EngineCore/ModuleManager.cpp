#include "ModuleManager.h"

namespace BE
{
    FModuleManager& FModuleManager::Get()
    {
        static FModuleManager Instance;
        return Instance;
    }

    bool FModuleManager::LoadAllConfiguredModules(bool /*bIsEditor*/)
    {
        // TODO: implement actual *.bemodule scanning and DLL loading.
        // For now just pretend everything is loaded successfully.
        return true;
    }

    void FModuleManager::ShutdownModules()
    {
        // TODO: unload modules and clean up state when real module system is implemented.
    }
}

