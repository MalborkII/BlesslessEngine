#include "Transform.h"

namespace BE
{
    Transform::Transform() = default;

    void Transform::SetParent(Transform* parent)
    {
        Parent = parent;
    }

    void Transform::SetLocalPosition(const Vec3& p)
    {
        LocalPosition = p;
    }

    void Transform::SetLocalRotation(const Quat& r)
    {
        LocalRotation = r;
    }

    void Transform::SetLocalScale(const Vec3& s)
    {
        LocalScale = s;
    }

    Vec3 Transform::GetWorldPosition() const
    {
        if (Parent)
        {
            Vec3 parentPos = Parent->GetWorldPosition();
            return Vec3{
                parentPos.x + LocalPosition.x,
                parentPos.y + LocalPosition.y,
                parentPos.z + LocalPosition.z
            };
        }
        return LocalPosition;
    }
}

