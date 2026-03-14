#include "ModuleManager.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#ifdef _WIN32
#  include <windows.h>
#endif

namespace BE
{
    namespace
    {
#ifdef _WIN32
        struct LoadedModule
        {
            std::string   Name;
            HMODULE       Handle { nullptr };
            BE_ModuleInfo Info {};
            using StartupFn  = bool(*)();
            using ShutdownFn = void(*)();
            StartupFn  Startup  { nullptr };
            ShutdownFn Shutdown { nullptr };
        };

        std::vector<LoadedModule> GLoadedModules;

        std::string GetParentDirectory(const std::string& path)
        {
            std::size_t slash = path.find_last_of("/\\");
            if (slash == std::string::npos)
                return {};
            return path.substr(0, slash);
        }

        std::string ReadFileToString(const std::string& path)
        {
            std::ifstream f(path);
            if (!f)
                return {};
            std::string content((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());
            return content;
        }

        std::vector<std::string> ParseModulesFromProject(const std::string& projectPath)
        {
            std::vector<std::string> result;
            std::string json = ReadFileToString(projectPath);
            if (json.empty())
                return result;

            std::size_t pos = json.find("\"Modules\"");
            if (pos == std::string::npos)
                return result;
            pos = json.find('[', pos);
            if (pos == std::string::npos)
                return result;
            std::size_t end = json.find(']', pos);
            if (end == std::string::npos)
                return result;

            std::string arrayPart = json.substr(pos, end - pos);
            std::size_t q = 0;
            while ((q = arrayPart.find('\"', q)) != std::string::npos)
            {
                std::size_t q2 = arrayPart.find('\"', q + 1);
                if (q2 == std::string::npos)
                    break;
                std::string name = arrayPart.substr(q + 1, q2 - q - 1);
                if (!name.empty())
                    result.push_back(name);
                q = q2 + 1;
            }
            return result;
        }

        std::string ParseBinaryFromBemodule(const std::string& bemodulePath)
        {
            std::string json = ReadFileToString(bemodulePath);
            if (json.empty())
                return {};
            std::size_t pos = json.find("\"Binary\"");
            if (pos == std::string::npos)
                return {};
            pos = json.find(':', pos);
            if (pos == std::string::npos)
                return {};
            std::size_t q1 = json.find('\"', pos);
            if (q1 == std::string::npos)
                return {};
            std::size_t q2 = json.find('\"', q1 + 1);
            if (q2 == std::string::npos)
                return {};
            return json.substr(q1 + 1, q2 - q1 - 1);
        }

        bool LoadModuleDll(const std::string& moduleName,
                           const std::string& moduleRoot)
        {
            std::string bemodulePath = moduleRoot + "\\" + moduleName + ".bemodule";
            std::string binaryName   = ParseBinaryFromBemodule(bemodulePath);
            if (binaryName.empty())
            {
                return false;
            }

            std::string dllPath = moduleRoot + "\\Binaries\\" + binaryName;
            HMODULE handle = LoadLibraryA(dllPath.c_str());
            if (!handle)
            {
                return false;
            }

            auto startup  = reinterpret_cast<LoadedModule::StartupFn>(
                GetProcAddress(handle, "BE_Module_Startup"));
            auto shutdown = reinterpret_cast<LoadedModule::ShutdownFn>(
                GetProcAddress(handle, "BE_Module_Shutdown"));
            auto getInfo  = reinterpret_cast<const BE_ModuleInfo*(*)()>(
                GetProcAddress(handle, "BE_Module_GetInfo"));

            LoadedModule lm;
            lm.Name    = moduleName;
            lm.Handle  = handle;
            lm.Startup = startup;
            lm.Shutdown = shutdown;
            if (getInfo)
            {
                const BE_ModuleInfo* info = getInfo();
                if (info)
                    lm.Info = *info;
            }

            if (lm.Startup)
            {
                if (!lm.Startup())
                {
                    if (lm.Shutdown)
                        lm.Shutdown();
                    FreeLibrary(handle);
                    return false;
                }
            }

            GLoadedModules.push_back(std::move(lm));
            return true;
        }
#endif
    }

    FModuleManager& FModuleManager::Get()
    {
        static FModuleManager Instance;
        return Instance;
    }

    bool FModuleManager::LoadAllConfiguredModules(bool /*bIsEditor*/, const char* projectFilePath)
    {
#ifdef _WIN32
        if (!projectFilePath || !projectFilePath[0])
            return true;

        std::string projectPath(projectFilePath);
        std::string projectDir = GetParentDirectory(projectPath);
        std::string engineRoot = GetParentDirectory(projectDir); // .../BlesslessEngine
        if (engineRoot.empty())
            return true;

        std::vector<std::string> modules = ParseModulesFromProject(projectPath);
        if (modules.empty())
            return true;

        std::string modulesRoot = engineRoot + "\\Modules";

        for (const std::string& name : modules)
        {
            std::string moduleRoot = modulesRoot + "\\" + name;
            LoadModuleDll(name, moduleRoot);
            // Ошибки загрузки конкретного модуля игнорируются, чтобы отсутствие DLL не ломало движок.
        }
#endif
        return true;
    }

    void FModuleManager::ShutdownModules()
    {
#ifdef _WIN32
        for (auto it = GLoadedModules.rbegin(); it != GLoadedModules.rend(); ++it)
        {
            if (it->Shutdown)
                it->Shutdown();
            if (it->Handle)
                FreeLibrary(it->Handle);
        }
        GLoadedModules.clear();
#endif
    }
}

