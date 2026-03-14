using System;
using System.Numerics;
using System.Runtime.InteropServices;

namespace Blessless.Engine
{
    // C# обертки над C API из ManagedBridge.h
    internal static class Native
    {
        private const string DllName = "BlesslessEngine";

        // Scene
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong BE_CreateScene();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_DestroyScene(ulong scene);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_Scene_Update(ulong scene, float deltaTime);

        // GameObject
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong BE_Scene_CreateGameObject(ulong scene, string name);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_Scene_DestroyGameObject(ulong scene, ulong objectId);

        // Transform
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_GameObject_GetTransform(
            ulong objectId,
            float[] outPosition3,
            float[] outRotationQuat4,
            float[] outScale3);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_GameObject_SetTransform(
            ulong objectId,
            float[] position3,
            float[] rotationQuat4,
            float[] scale3);

        // Managed scripts callbacks registration
        internal delegate void ManagedScriptStartCallback(ulong instanceId);
        internal delegate void ManagedScriptUpdateCallback(ulong instanceId, float deltaTime);
        internal delegate void ManagedScriptDestroyCallback(ulong instanceId);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern void BE_SetManagedScriptCallbacks(
            ManagedScriptStartCallback onStart,
            ManagedScriptUpdateCallback onUpdate,
            ManagedScriptDestroyCallback onDestroy);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong BE_GameObject_AddManagedScript(
            ulong objectId,
            ulong managedInstanceId);
    }

    public readonly struct SceneHandle
    {
        public readonly ulong Id;
        internal SceneHandle(ulong id) => Id = id;
        public bool IsValid => Id != 0;
    }

    public readonly struct GameObjectHandle
    {
        public readonly ulong Id;
        internal GameObjectHandle(ulong id) => Id = id;
        public bool IsValid => Id != 0;
    }

    public static class SceneRuntime
    {
        public static SceneHandle Create()
            => new SceneHandle(Native.BE_CreateScene());

        public static void Destroy(SceneHandle scene)
            => Native.BE_DestroyScene(scene.Id);

        public static void Update(SceneHandle scene, float deltaTime)
            => Native.BE_Scene_Update(scene.Id, deltaTime);

        public static GameObjectHandle CreateGameObject(SceneHandle scene, string name)
            => new GameObjectHandle(Native.BE_Scene_CreateGameObject(scene.Id, name));

        public static void DestroyGameObject(SceneHandle scene, GameObjectHandle obj)
            => Native.BE_Scene_DestroyGameObject(scene.Id, obj.Id);

        public static void GetTransform(GameObjectHandle obj, out Vector3 position, out Quaternion rotation, out Vector3 scale)
        {
            var pos = new float[3];
            var rot = new float[4];
            var scl = new float[3];
            Native.BE_GameObject_GetTransform(obj.Id, pos, rot, scl);
            position = new Vector3(pos[0], pos[1], pos[2]);
            rotation = new Quaternion(rot[0], rot[1], rot[2], rot[3]);
            scale = new Vector3(scl[0], scl[1], scl[2]);
        }

        public static void SetTransform(GameObjectHandle obj, Vector3 position, Quaternion rotation, Vector3 scale)
        {
            var pos = new[] { position.X, position.Y, position.Z };
            var rot = new[] { rotation.X, rotation.Y, rotation.Z, rotation.W };
            var scl = new[] { scale.X, scale.Y, scale.Z };
            Native.BE_GameObject_SetTransform(obj.Id, pos, rot, scl);
        }
    }

    // Базовый класс для C#-скриптов; привязка к C++ будет добавлена позже.
    public abstract class ScriptBehaviour
    {
        public GameObjectHandle GameObject { get; internal set; }

        public virtual void OnStart() { }
        public virtual void OnUpdate(float deltaTime) { }
        public virtual void OnDestroy() { }
    }
}

using System;
using System.Numerics;
using System.Runtime.InteropServices;

namespace Blessless.Engine
{
    // C# обертки над C API из ManagedBridge.h
    internal static class Native
    {
        private const string DllName = "BlesslessEngine";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong BE_CreateWorld();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_DestroyWorld(ulong world);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong BE_World_SpawnActor(ulong world, string actorTypeName);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_World_DestroyActor(ulong world, ulong actorId);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_Actor_GetTransform(
            ulong actorId,
            float[] outPosition3,
            float[] outRotationQuat4,
            float[] outScale3);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void BE_Actor_SetTransform(
            ulong actorId,
            float[] position3,
            float[] rotationQuat4,
            float[] scale3);

        public delegate void ManagedTickCallback(float deltaTime);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern void BE_SetManagedTickCallback(ManagedTickCallback callback);
    }

    public readonly struct WorldHandle
    {
        public readonly ulong Id;
        internal WorldHandle(ulong id) => Id = id;
        public bool IsValid => Id != 0;
    }

    public readonly struct ActorHandle
    {
        public readonly ulong Id;
        internal ActorHandle(ulong id) => Id = id;
        public bool IsValid => Id != 0;
    }

    public static class Engine
    {
        public static WorldHandle CreateWorld()
            => new WorldHandle(Native.BE_CreateWorld());

        public static void DestroyWorld(WorldHandle world)
            => Native.BE_DestroyWorld(world.Id);

        public static ActorHandle SpawnActor(WorldHandle world, string actorTypeName)
            => new ActorHandle(Native.BE_World_SpawnActor(world.Id, actorTypeName));

        public static void DestroyActor(WorldHandle world, ActorHandle actor)
            => Native.BE_World_DestroyActor(world.Id, actor.Id);

        public static void GetTransform(ActorHandle actor, out Vector3 position, out Quaternion rotation, out Vector3 scale)
        {
            var pos = new float[3];
            var rot = new float[4];
            var scl = new float[3];
            Native.BE_Actor_GetTransform(actor.Id, pos, rot, scl);
            position = new Vector3(pos[0], pos[1], pos[2]);
            rotation = new Quaternion(rot[0], rot[1], rot[2], rot[3]);
            scale = new Vector3(scl[0], scl[1], scl[2]);
        }

        public static void SetTransform(ActorHandle actor, Vector3 position, Quaternion rotation, Vector3 scale)
        {
            var pos = new[] { position.X, position.Y, position.Z };
            var rot = new[] { rotation.X, rotation.Y, rotation.Z, rotation.W };
            var scl = new[] { scale.X, scale.Y, scale.Z };
            Native.BE_Actor_SetTransform(actor.Id, pos, rot, scl);
        }

        public static void RegisterManagedTick(Native.ManagedTickCallback callback)
            => Native.BE_SetManagedTickCallback(callback);
    }
}
