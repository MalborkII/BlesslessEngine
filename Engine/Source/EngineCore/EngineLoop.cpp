#include "EngineCore.h"
#include "ModuleManager.h"
#include "Scene.h"

namespace BE
{
    static EngineInitParams GInitParams{};
    static Scene*           GActiveScene { nullptr };

    bool Engine_PreInit(const EngineInitParams& params)
    {
        GInitParams = params;

        // TODO: load engine config, set up logging, allocators, paths, reflection, etc.
        // This file defines only high-level flow; concrete systems will be added later.

        return true;
    }

    bool Engine_LoadModules()
    {
        return FModuleManager::Get().LoadAllConfiguredModules(
            GInitParams.bIsEditor,
            GInitParams.ProjectFilePath
        );
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
        // TODO: implement main loop driving scenes, renderer and C# runtime.
        return 0;
    }

    Scene* Engine_CreateScene()
    {
        if (GActiveScene)
        {
            delete GActiveScene;
            GActiveScene = nullptr;
        }
        GActiveScene = new Scene();
        return GActiveScene;
    }

    void Engine_DestroyScene(Scene* scene)
    {
        if (!scene)
            return;

        if (scene == GActiveScene)
        {
            delete GActiveScene;
            GActiveScene = nullptr;
        }
        else
        {
            delete scene;
        }
    }

    void Engine_Shutdown()
    {
        Engine_DestroyScene(GActiveScene);
        FModuleManager::Get().ShutdownModules();
        // TODO: tear down core systems, logging, etc.
    }
}

