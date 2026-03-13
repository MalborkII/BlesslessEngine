#pragma once

// C API, через который C# runtime (Game.dll) взаимодействует с C++ ядром.
// Предполагается использование P/Invoke с dllimport на стороне C#.

extern "C"
{
    // Типы-идентификаторы для объектов в ядре.
    typedef unsigned long long BE_ObjectId;
    typedef unsigned long long BE_WorldId;

    // Общие функции ядра
    __declspec(dllexport) BE_WorldId BE_CreateWorld();
    __declspec(dllexport) void       BE_DestroyWorld(BE_WorldId world);

    // Актеры и компоненты
    __declspec(dllexport) BE_ObjectId BE_World_SpawnActor(BE_WorldId world, const char* actorTypeName);
    __declspec(dllexport) void        BE_World_DestroyActor(BE_WorldId world, BE_ObjectId actorId);

    // Трансформации
    __declspec(dllexport) void BE_Actor_GetTransform(
        BE_ObjectId actorId,
        float* outPosition3,
        float* outRotationQuat4,
        float* outScale3
    );

    __declspec(dllexport) void BE_Actor_SetTransform(
        BE_ObjectId actorId,
        const float* position3,
        const float* rotationQuat4,
        const float* scale3
    );

    // Обновление (tick) из C++
    // Вызывается ядром каждый кадр для уведомления C# слоя.
    typedef void(__stdcall* BE_Managed_TickCallback)(float deltaTime);

    __declspec(dllexport) void BE_SetManagedTickCallback(BE_Managed_TickCallback callback);
}

