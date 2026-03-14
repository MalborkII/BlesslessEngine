#pragma once

#include "Engine.h"
#include "ModuleInterface.h"

// High-level C++ module manager responsible for loading/unloading C++ DLLs
// based on *.bemodule descriptors and engine/project configuration.

namespace BE
{
    struct ModuleDescriptor
    {
        const char* Name;
        const char* Type;              // "Engine", "Runtime", "Editor", "Game"
        const char* PathToBinary;      // Path to DLL
        const char* MinEngineVersion;  // Optional
        const char* MaxEngineVersion;  // Optional
        bool        bEditorOnly { false };
        bool        bRuntimeOnly { false };
        bool        bRequiredOnStart { false };
    };

    class FModuleManager
    {
    public:
        // Singleton-style access (simple for initial version).
        static FModuleManager& Get();

        // Discover descriptors in Modules/* and project modules,
        // resolve dependencies and load required modules.
        bool LoadAllConfiguredModules(bool bIsEditor, const char* projectFilePath);

        // Shutdown in reverse dependency order.
        void ShutdownModules();
    };
}

