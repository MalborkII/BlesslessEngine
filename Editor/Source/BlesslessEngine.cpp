/**
 * BlesslessEngine.exe
 * Точка входа движка и Python-редактора.
 *
 * Поток:
 *  - BlesslessEngine.exe "<path to project.bless>"
 *  - Engine_PreInit -> Engine_LoadModules -> (плейсхолдер загрузки C#)
 *  - Engine_InitEditor -> Engine_OpenProject
 *  - Запуск Python-редактора (Editor/Python/main.py)
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <cstdlib>

#include "Engine.h"

static std::string GetExePath() {
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) return "";
    return std::string(buf);
}

// Каталог движка: exe лежит в Binaries, поднимаемся на один уровень.
static std::string GetEngineRoot() {
    std::string exe = GetExePath();
    size_t pos = exe.find_last_of("\\/");
    if (pos == std::string::npos) return "";
    std::string dir = exe.substr(0, pos); // Binaries
    pos = dir.find_last_of("\\/");
    if (pos == std::string::npos) return "";
    return dir.substr(0, pos); // корень репозитория BlesslessEngine
}

static std::string FindPython() {
    const char* py = std::getenv("PYTHON");
    if (py && py[0]) return py;
    const char* path = std::getenv("PATH");
    if (!path) return "python";
    std::string pathCopy(path);
    std::string needle = "python.exe";
    size_t start = 0;
    for (;;) {
        size_t end = pathCopy.find(';', start);
        std::string dir = end == std::string::npos ? pathCopy.substr(start) : pathCopy.substr(start, end - start);
        if (!dir.empty() && *dir.rbegin() != '\\') dir += '\\';
        std::string candidate = dir + needle;
        if (GetFileAttributesA(candidate.c_str()) != INVALID_FILE_ATTRIBUTES)
            return candidate;
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return "python";
}

static bool LaunchPythonEditor(const std::string& engineRoot, const std::string& projectPath) {
    std::string mainPy = engineRoot + "\\Editor\\Python\\main.py";
    if (GetFileAttributesA(mainPy.c_str()) == INVALID_FILE_ATTRIBUTES) {
        MessageBoxA(NULL, ("Не найден скрипт редактора: " + mainPy).c_str(), "BlesslessEngine", MB_OK | MB_ICONERROR);
        return false;
    }

    std::string pythonExe = FindPython();
    std::string cmdLine = "\"" + pythonExe + "\" \"" + mainPy + "\" \"" + projectPath + "\"";
    std::vector<char> cmdCopy(cmdLine.begin(), cmdLine.end());
    cmdCopy.push_back('\0');
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    BOOL ok = CreateProcessA(
        NULL,
        cmdCopy.data(),
        NULL, NULL, FALSE, 0, NULL,
        engineRoot.c_str(),
        &si, &pi);
    if (!ok) {
        MessageBoxA(NULL, "Не удалось запустить Python. Убедитесь, что python в PATH.", "BlesslessEngine", MB_OK | MB_ICONERROR);
        return false;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, int) {
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::string projectPath;
    if (argv && argc >= 2 && argv[1] && argv[1][0]) {
        char buf[MAX_PATH * 2];
        if (WideCharToMultiByte(CP_UTF8, 0, argv[1], -1, buf, sizeof(buf), NULL, NULL) > 0)
            projectPath = buf;
    }
    if (argv) LocalFree(argv);

    std::string engineRoot = GetEngineRoot();
    if (engineRoot.empty()) {
        MessageBoxA(NULL, "Не удалось определить корень движка.", "BlesslessEngine", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (projectPath.empty()) {
        projectPath = engineRoot + "\\Projects\\SampleProject\\project.bless";
    }

    // Инициализация ядра движка
    BE::EngineInitParams params{};
    std::string engineConfigPath = engineRoot + "\\Engine\\Config\\EngineModules.json";
    params.ProjectFilePath  = projectPath.c_str();
    params.EngineConfigPath = engineConfigPath.c_str();
    params.bIsEditor        = true;

    if (!BE::Engine_PreInit(params)) {
        MessageBoxA(NULL, "Engine_PreInit failed.", "BlesslessEngine", MB_OK | MB_ICONERROR);
        return 2;
    }

    if (!BE::Engine_LoadModules()) {
        MessageBoxA(NULL, "Engine_LoadModules failed.", "BlesslessEngine", MB_OK | MB_ICONERROR);
        return 3;
    }

    // TODO: загрузка/инициализация C# runtime и игровых сборок (C# скрипты).

    if (!BE::Engine_InitEditor()) {
        MessageBoxA(NULL, "Engine_InitEditor failed.", "BlesslessEngine", MB_OK | MB_ICONERROR);
        return 4;
    }

    if (!BE::Engine_OpenProject(projectPath.c_str())) {
        MessageBoxA(NULL, "Engine_OpenProject failed.", "BlesslessEngine", MB_OK | MB_ICONERROR);
        return 5;
    }

    // Запуск Python-редактора поверх инициализированного ядра
    if (!LaunchPythonEditor(engineRoot, projectPath))
        return 6;

    return 0;
}
