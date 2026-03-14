#pragma once

#include "Component.h"

namespace BE
{
    class Camera : public Component
    {
    public:
        ComponentType GetType() const override { return ComponentType::Camera; }

        float FovYDegrees { 60.0f };
        float NearPlane   { 0.1f };
        float FarPlane    { 1000.0f };
    };

    class MeshRenderer : public Component
    {
    public:
        ComponentType GetType() const override { return ComponentType::MeshRenderer; }

        // Placeholders for mesh/material handles.
        void* MeshHandle { nullptr };
        void* MaterialHandle { nullptr };
    };

    class Light : public Component
    {
    public:
        enum class Type : uint8_t
        {
            Directional,
            Point,
            Spot
        };

        ComponentType GetType() const override { return ComponentType::Light; }

        Type  LightType { Type::Directional };
        float Intensity { 1.0f };
    };
}

