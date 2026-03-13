#pragma once

// C API, через который C# runtime (Game.dll) взаимодействует с C++ ядром.
// Предполагается использование P/Invoke с dllimport на стороне C#.

extern "C"
{
    // Типы-идентификаторы для сущностей в сцене.
    typedef unsigned long long BE_SceneId;
    typedef unsigned long long BE_ObjectId;

    // Scene lifecycle
    __declspec(dllexport) BE_SceneId BE_CreateScene();
    __declspec(dllexport) void       BE_DestroyScene(BE_SceneId scene);
    __declspec(dllexport) void       BE_Scene_Update(BE_SceneId scene, float deltaTime);

    // GameObject lifecycle
    __declspec(dllexport) BE_ObjectId BE_Scene_CreateGameObject(BE_SceneId scene, const char* name);
    __declspec(dllexport) void        BE_Scene_DestroyGameObject(BE_SceneId scene, BE_ObjectId objectId);

    // Transform access
    __declspec(dllexport) void BE_GameObject_GetTransform(
        BE_ObjectId objectId,
        float* outPosition3,
        float* outRotationQuat4,
        float* outScale3
    );

    __declspec(dllexport) void BE_GameObject_SetTransform(
        BE_ObjectId objectId,
        const float* position3,
        const float* rotationQuat4,
        const float* scale3
    );

    // Managed (C#) scripts
    typedef void(__stdcall* BE_Managed_ScriptStartCallback)(unsigned long long instanceId);
    typedef void(__stdcall* BE_Managed_ScriptUpdateCallback)(unsigned long long instanceId, float deltaTime);
    typedef void(__stdcall* BE_Managed_ScriptDestroyCallback)(unsigned long long instanceId);

    // Регистрирует коллбеки, которые C++ будет вызывать для C# скриптов.
    __declspec(dllexport) void BE_SetManagedScriptCallbacks(
        BE_Managed_ScriptStartCallback   onStart,
        BE_Managed_ScriptUpdateCallback  onUpdate,
        BE_Managed_ScriptDestroyCallback onDestroy
    );

    // Привязывает C#-инстанс (managedInstanceId) как Script-компонент к GameObject.
    __declspec(dllexport) BE_ObjectId BE_GameObject_AddManagedScript(
        BE_ObjectId        objectId,
        unsigned long long managedInstanceId
    );
}

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

