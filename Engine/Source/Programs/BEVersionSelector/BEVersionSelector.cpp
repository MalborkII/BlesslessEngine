/**
 * BEVersionSelector.exe
 * Запускается по двойному клику на project.bless (ассоциация файлов Windows).
 * Читает EngineVersion из project.bless, находит BlesslessEngine.exe для этой версии,
 * запускает: BlesslessEngine.exe "<путь к project.bless>"
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <fstream>

static std::string GetExeDirectory() {
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) return "";
    std::string path(buf);
    size_t last = path.find_last_of("\\/");
    return last != std::string::npos ? path.substr(0, last) : path;
}

/** Извлекает значение "EngineVersion" из JSON (упрощённый разбор). */
static std::string ReadEngineVersion(const std::string& projectPath) {
    std::ifstream f(projectPath);
    if (!f) return "0.1.0";
    std::string line;
    while (std::getline(f, line)) {
        size_t pos = line.find("\"EngineVersion\"");
        if (pos == std::string::npos) continue;
        pos = line.find(':', pos);
        if (pos == std::string::npos) continue;
        size_t start = line.find('"', pos);
        if (start == std::string::npos) continue;
        start += 1;
        size_t end = line.find('"', start);
        if (end == std::string::npos) continue;
        return line.substr(start, end - start);
    }
    return "0.1.0";
}

/** Путь к BlesslessEngine.exe: Binaries/Win64/BlesslessEngine.exe (рядом с BEVersionSelector). */
static std::string GetBlesslessEnginePath(const std::string& version) {
    std::string exeDir = GetExeDirectory();
    return exeDir + "\\BlesslessEngine.exe";
}

static bool LaunchBlesslessEngine(const std::string& editorExe, const std::string& projectPath) {
    std::string cmdLine = "\"" + editorExe + "\" \"" + projectPath + "\"";
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    std::vector<char> cmdCopy(cmdLine.begin(), cmdLine.end());
    cmdCopy.push_back('\0');
    if (!CreateProcessA(
        editorExe.c_str(),
        cmdCopy.data(),
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        return false;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::string projectPath;
    if (argv && argc >= 2 && argv[1] && argv[1][0]) {
        char buf[MAX_PATH * 2];
        if (WideCharToMultiByte(CP_UTF8, 0, argv[1], -1, buf, (int)sizeof(buf), NULL, NULL) > 0)
            projectPath = buf;
    }
    if (argv) LocalFree(argv);

    if (projectPath.empty()) {
        MessageBoxW(NULL, L"Запустите двойным кликом по файлу project.bless.", L"BEVersionSelector", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    std::string version = ReadEngineVersion(projectPath);
    std::string editorExe = GetBlesslessEnginePath(version);

    if (GetFileAttributesA(editorExe.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::string msg = "Не найден редактор: " + editorExe;
        MessageBoxA(NULL, msg.c_str(), "BEVersionSelector", MB_OK | MB_ICONERROR);
        return 2;
    }

    if (!LaunchBlesslessEngine(editorExe, projectPath)) {
        MessageBoxA(NULL, "Не удалось запустить BlesslessEngine.exe", "BEVersionSelector", MB_OK | MB_ICONERROR);
        return 3;
    }
    return 0;
}
