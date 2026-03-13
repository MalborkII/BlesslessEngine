#include "GameObject.h"

#include "Script.h"

namespace BE
{
    GameObject::GameObject(GameObjectId id, Scene* ownerScene, const std::string& name)
        : Id(id)
        , OwnerScene(ownerScene)
        , Name(name)
    {
        TransformStorage = std::make_unique<Transform>();
        TransformComponent = TransformStorage.get();
        TransformComponent->SetOwner(this);
        TransformComponent->OnAttach();
        NotifyComponentAdded(TransformComponent);
    }

    GameObject::~GameObject()
    {
        // Detach components in reverse order.
        for (auto it = Components.rbegin(); it != Components.rend(); ++it)
        {
            Component* c = it->get();
            NotifyComponentRemoved(c);
            c->OnDetach();
        }

        if (TransformComponent)
        {
            NotifyComponentRemoved(TransformComponent);
            TransformComponent->OnDetach();
        }
    }

    void GameObject::NotifyComponentAdded(Component* c)
    {
        if (OwnerScene)
        {
            // Defined in Scene.cpp
            extern void Scene_OnComponentAdded(Scene* scene, GameObject* obj, Component* c);
            Scene_OnComponentAdded(OwnerScene, this, c);
        }
    }

    void GameObject::NotifyComponentRemoved(Component* c)
    {
        if (OwnerScene)
        {
            extern void Scene_OnComponentRemoved(Scene* scene, GameObject* obj, Component* c);
            Scene_OnComponentRemoved(OwnerScene, this, c);
        }
    }
}

