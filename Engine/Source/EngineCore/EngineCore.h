#pragma once

// Core entry points for BlesslessEngine C++ static library.
// EngineLoop and module manager are declared here at a high level.

namespace BE
{
    struct EngineInitParams
    {
        const char* ProjectFilePath;   // path to project.bless
        const char* EngineConfigPath;  // optional path to engine config
        bool        bIsEditor;         // true for editor, false for standalone runtime
    };

    // High-level engine loop phases.
    enum class EngineRunMode
    {
        Editor,
        Game
    };

    // Called very early by host (BlessEditor.exe or standalone game).
    bool Engine_PreInit(const EngineInitParams& params);

    // Loads all configured C++ modules (DLLs) based on engine and project configs.
    bool Engine_LoadModules();

    // Initializes editor-specific systems (called only when running editor).
    bool Engine_InitEditor();

    // Initializes runtime-only systems (standalone game / PIE world).
    bool Engine_InitRuntime();

    // Opens a project, reading project.bless and loading assets/worlds.
    bool Engine_OpenProject(const char* projectFilePath);

    // Main ticking loop entrypoint (for standalone runtime).
    int Engine_Run(EngineRunMode mode);

    // Clean shutdown for engine and all loaded modules.
    void Engine_Shutdown();
}

