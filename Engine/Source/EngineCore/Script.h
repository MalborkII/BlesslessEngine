#pragma once

#include "Component.h"

namespace BE
{
    class Script : public Component
    {
    public:
        ComponentType GetType() const override { return ComponentType::Script; }

        virtual void OnStart() {}
        virtual void OnUpdate(float /*deltaTime*/) {}
        virtual void OnDestroy() {}

        bool HasStarted() const { return bStarted; }
        void MarkStarted() { bStarted = true; }

    private:
        bool bStarted { false };
    };
}

