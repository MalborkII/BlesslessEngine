#include "Scene.h"

#include <algorithm>

namespace BE
{
    Scene::Scene() = default;

    Scene::~Scene() = default;

    GameObject* Scene::CreateGameObject(const std::string& name)
    {
        auto obj = std::make_unique<GameObject>(NextId++, this, name);
        GameObject* raw = obj.get();
        GameObjects.emplace_back(std::move(obj));
        return raw;
    }

    void Scene::DestroyGameObject(GameObject* obj)
    {
        if (!obj) return;
        obj->MarkForDestroy();
        PendingDestroy.push_back(obj);
    }

    void Scene::Update(float deltaTime)
    {
        UpdateScripts(deltaTime);
        UpdateTransforms();
        CleanupDestroyed();
    }

    void Scene::OnComponentAdded(GameObject* /*obj*/, Component* c)
    {
        if (auto s = dynamic_cast<Script*>(c))
        {
            Scripts.push_back(s);
        }
        else if (auto cam = dynamic_cast<Camera*>(c))
        {
            Cameras.push_back(cam);
        }
        else if (auto mr = dynamic_cast<MeshRenderer*>(c))
        {
            MeshRenderers.push_back(mr);
        }
        else if (auto light = dynamic_cast<Light*>(c))
        {
            Lights.push_back(light);
        }
    }

    void Scene::OnComponentRemoved(GameObject* /*obj*/, Component* c)
    {
        auto removePtr = [c](auto& vec)
        {
            vec.erase(
                std::remove(vec.begin(), vec.end(), c),
                vec.end());
        };

        if (dynamic_cast<Script*>(c))
        {
            removePtr(Scripts);
        }
        else if (dynamic_cast<Camera*>(c))
        {
            removePtr(Cameras);
        }
        else if (dynamic_cast<MeshRenderer*>(c))
        {
            removePtr(MeshRenderers);
        }
        else if (dynamic_cast<Light*>(c))
        {
            removePtr(Lights);
        }
    }

    void Scene::UpdateScripts(float deltaTime)
    {
        for (Script* s : Scripts)
        {
            if (!s || !s->GetOwner() || s->GetOwner()->IsPendingDestroy())
                continue;

            if (!s->HasStarted())
            {
                s->OnStart();
                s->MarkStarted();
            }
            s->OnUpdate(deltaTime);
        }
    }

    void Scene::UpdateTransforms()
    {
        // For now rely on recursive GetWorldPosition in Transform; nothing to do here
        (void)0;
    }

    void Scene::CleanupDestroyed()
    {
        if (PendingDestroy.empty())
            return;

        GameObjects.erase(
            std::remove_if(
                GameObjects.begin(),
                GameObjects.end(),
                [](const std::unique_ptr<GameObject>& obj)
                {
                    return obj->IsPendingDestroy();
                }),
            GameObjects.end());

        PendingDestroy.clear();
    }

    void Scene_OnComponentAdded(Scene* scene, GameObject* obj, Component* c)
    {
        if (scene)
            scene->OnComponentAdded(obj, c);
    }

    void Scene_OnComponentRemoved(Scene* scene, GameObject* obj, Component* c)
    {
        if (scene)
            scene->OnComponentRemoved(obj, c);
    }
}

