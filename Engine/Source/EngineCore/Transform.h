#pragma once

#include "Component.h"

namespace BE
{
    struct Vec3
    {
        float x { 0.0f };
        float y { 0.0f };
        float z { 0.0f };
    };

    struct Quat
    {
        float x { 0.0f };
        float y { 0.0f };
        float z { 0.0f };
        float w { 1.0f };
    };

    class Transform final : public Component
    {
    public:
        Transform();

        ComponentType GetType() const override { return ComponentType::Transform; }

        Transform* GetParent() const { return Parent; }
        void       SetParent(Transform* parent);

        Vec3 GetLocalPosition() const { return LocalPosition; }
        void SetLocalPosition(const Vec3& p);

        Quat GetLocalRotation() const { return LocalRotation; }
        void SetLocalRotation(const Quat& r);

        Vec3 GetLocalScale() const { return LocalScale; }
        void SetLocalScale(const Vec3& s);

        // For now expose world transform as simple getters; actual matrix math can be added later.
        Vec3 GetWorldPosition() const;

    private:
        Transform* Parent { nullptr };
        // Children management is handled by Scene/GameObject when needed.

        Vec3 LocalPosition {};
        Quat LocalRotation {};
        Vec3 LocalScale { 1.0f, 1.0f, 1.0f };
    };
}

