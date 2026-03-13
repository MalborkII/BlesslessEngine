#include "LoggerModule.h"

#include <cstdio>
#include <cstring>
#include <string>

#ifdef _WIN32
#  include <direct.h>
#endif

namespace
{
    std::string g_LogFilePath;

    void EnsureDirectories(const std::string& projectDir)
    {
        std::string saved = projectDir + "\\Saved";
        std::string logs  = saved + "\\Logs";
#ifdef _WIN32
        _mkdir(saved.c_str());
        _mkdir(logs.c_str());
#endif
        g_LogFilePath = logs + "\\Blessless.log";
    }
}

extern "C"
{
    void BE_Logger_Initialize(const char* projectDir)
    {
        if (!projectDir || !projectDir[0])
            return;
        EnsureDirectories(projectDir);

        // Создаём/очищаем файл.
        if (!g_LogFilePath.empty())
        {
            std::FILE* f = nullptr;
            fopen_s(&f, g_LogFilePath.c_str(), "w");
            if (f)
                std::fclose(f);
        }
    }

    void BE_Logger_Log(const char* channel, const char* message)
    {
        if (g_LogFilePath.empty() || !message)
            return;

        std::FILE* f = nullptr;
        fopen_s(&f, g_LogFilePath.c_str(), "a");
        if (!f)
            return;

        if (channel && channel[0])
            std::fprintf(f, "[%s] %s\n", channel, message);
        else
            std::fprintf(f, "%s\n", message);

        std::fclose(f);
    }
}

