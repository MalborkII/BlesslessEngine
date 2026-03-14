#pragma once

// C API для Python-редактора.
// Реализуется в C++ ядре и биндится в Python (через C-расширение, CFFI или pybind11).

extern "C"
{
    typedef unsigned long long BE_ObjectId;
    typedef unsigned long long BE_WorldId;

    // Открытие проекта из редактора
    __declspec(dllexport) bool BE_Editor_OpenProject(const char* projectFilePath);

    // Доступ к сценам/мирам
    __declspec(dllexport) BE_WorldId BE_Editor_GetActiveWorld();

    // Выбор объектов
    __declspec(dllexport) void BE_Editor_SelectObject(BE_ObjectId objectId);
    __declspec(dllexport) void BE_Editor_ClearSelection();

    // Команды редактора
    __declspec(dllexport) void BE_Editor_Command_SaveWorld(BE_WorldId world);
    __declspec(dllexport) void BE_Editor_Command_ImportAsset(const char* filePath);
}

