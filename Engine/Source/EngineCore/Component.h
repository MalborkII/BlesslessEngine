#pragma once

#include <cstdint>

namespace BE
{
    class GameObject;

    enum class ComponentType : uint8_t
    {
        Transform,
        Script,
        Camera,
        MeshRenderer,
        Light,
        Custom
    };

    class Component
    {
    public:
        virtual ~Component() = default;

        GameObject* GetOwner() const { return Owner; }
        void        SetOwner(GameObject* owner) { Owner = owner; }

        virtual ComponentType GetType() const = 0;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

    protected:
        GameObject* Owner { nullptr };
    };
}

