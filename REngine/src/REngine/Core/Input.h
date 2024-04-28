#pragma once

#include "REngine/Core/Core.h"

namespace REngine
{
    class Input
    {
    public:
        inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_instance->GetMousePositionImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;

    private:
        static Scope<Input> s_instance;
    };
}