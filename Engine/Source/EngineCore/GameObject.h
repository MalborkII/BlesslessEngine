#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "Component.h"
#include "Transform.h"

namespace BE
{
    class Scene;

    using GameObjectId = std::uint64_t;

    class GameObject
    {
    public:
        GameObject(GameObjectId id, Scene* ownerScene, const std::string& name);
        ~GameObject();

        GameObjectId   GetId() const { return Id; }
        const std::string& GetName() const { return Name; }

        bool IsActive() const { return bActive; }
        void SetActive(bool active) { bActive = active; }

        Scene*     GetScene() const { return OwnerScene; }
        Transform& GetTransform() const { return *TransformComponent; }

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            comp->SetOwner(this);
            T* raw = comp.get();
            Components.emplace_back(std::move(comp));
            raw->OnAttach();
            NotifyComponentAdded(raw);
            return raw;
        }

        template<typename T>
        T* GetComponent() const
        {
            for (const auto& c : Components)
            {
                if (auto casted = dynamic_cast<T*>(c.get()))
                    return casted;
            }
            return nullptr;
        }

        void MarkForDestroy() { bPendingDestroy = true; }
        bool IsPendingDestroy() const { return bPendingDestroy; }

    private:
        friend class Scene;

        void NotifyComponentAdded(Component* c);
        void NotifyComponentRemoved(Component* c);

        GameObjectId Id { 0 };
        Scene*       OwnerScene { nullptr };
        std::string  Name;
        bool         bActive { true };
        bool         bPendingDestroy { false };

        std::unique_ptr<Transform>                 TransformStorage;
        Transform*                                 TransformComponent { nullptr };
        std::vector<std::unique_ptr<Component>>    Components;
    };
}

