#include "EngineCore.h"
#include "ModuleManager.h"

namespace BE
{
    static EngineInitParams GInitParams{};

    bool Engine_PreInit(const EngineInitParams& params)
    {
        GInitParams = params;

        // TODO: load engine config, set up logging, allocators, paths, reflection, etc.
        // This file defines only high-level flow; concrete systems will be added later.

        return true;
    }

    bool Engine_LoadModules()
    {
        return FModuleManager::Get().LoadAllConfiguredModules(GInitParams.bIsEditor);
    }

    bool Engine_InitEditor()
    {
        // TODO: initialize editor-specific subsystems (selection, viewport, asset browser, undo/redo).
        return true;
    }

    bool Engine_InitRuntime()
    {
        // TODO: initialize runtime-only systems (game loop, physics world, etc.).
        return true;
    }

    bool Engine_OpenProject(const char* projectFilePath)
    {
        (void)projectFilePath;
        // TODO: parse project.bless, load assets, worlds and game assemblies.
        return true;
    }

    int Engine_Run(EngineRunMode mode)
    {
        (void)mode;
        // TODO: implement main loop driving worlds and ticking C# runtime and editor.
        return 0;
    }

    void Engine_Shutdown()
    {
        FModuleManager::Get().ShutdownModules();
        // TODO: tear down core systems, logging, etc.
    }
}

