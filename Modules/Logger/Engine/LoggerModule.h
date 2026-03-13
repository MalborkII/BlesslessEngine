#pragma once

// C API для логирующего модуля Logger.
// Реализуется в BE-Logger.dll и вызывается из ядра движка.

extern "C"
{
    // Инициализация логгера. projectDir — директория проекта (рядом с project.bless).
    __declspec(dllexport) void BE_Logger_Initialize(const char* projectDir);

    // Записать строку лога в файл.
    __declspec(dllexport) void BE_Logger_Log(const char* channel, const char* message);
}

