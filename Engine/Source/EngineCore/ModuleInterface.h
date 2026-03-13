#pragma once

// Common C ABI for all C++ modules (Engine and feature modules).
// DLLs implement these functions and are discovered/managed by FModuleManager.

extern "C"
{
    struct BE_ModuleInfo
    {
        const char* Name;             // Unique module name
        const char* Type;             // "Engine", "Runtime", "Editor", "Game"
        const char* Version;          // Optional semantic version string
        const char* MinEngineVersion; // Optional minimum engine version
        const char* MaxEngineVersion; // Optional maximum engine version
    };

    // Called once when the module is loaded.
    __declspec(dllexport) bool BE_Module_Startup();

    // Called once before the module is unloaded.
    __declspec(dllexport) void BE_Module_Shutdown();

    // Returns static information about the module.
    __declspec(dllexport) const BE_ModuleInfo* BE_Module_GetInfo();
}

