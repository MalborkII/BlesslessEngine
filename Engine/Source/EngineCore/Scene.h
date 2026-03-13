#pragma once

#include <memory>
#include <vector>
#include <string>

#include "GameObject.h"
#include "Script.h"
#include "RenderComponents.h"

namespace BE
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        GameObject* CreateGameObject(const std::string& name);
        void        DestroyGameObject(GameObject* obj);

        void Update(float deltaTime);

        const std::vector<Camera*>&       GetCameras() const { return Cameras; }
        const std::vector<MeshRenderer*>& GetMeshRenderers() const { return MeshRenderers; }
        const std::vector<Light*>&        GetLights() const { return Lights; }

    private:
        friend void ::BE::Scene_OnComponentAdded(Scene* scene, GameObject* obj, Component* c);
        friend void ::BE::Scene_OnComponentRemoved(Scene* scene, GameObject* obj, Component* c);

        void OnComponentAdded(GameObject* obj, Component* c);
        void OnComponentRemoved(GameObject* obj, Component* c);

        void UpdateScripts(float deltaTime);
        void UpdateTransforms();
        void CleanupDestroyed();

        std::vector<std::unique_ptr<GameObject>> GameObjects;
        std::vector<GameObject*>                 PendingDestroy;

        std::vector<Script*>       Scripts;
        std::vector<Camera*>       Cameras;
        std::vector<MeshRenderer*> MeshRenderers;
        std::vector<Light*>        Lights;

        GameObjectId NextId { 1 };
    };

    // C-style helpers used from GameObject.cpp to avoid circular includes.
    void Scene_OnComponentAdded(Scene* scene, GameObject* obj, Component* c);
    void Scene_OnComponentRemoved(Scene* scene, GameObject* obj, Component* c);
}

