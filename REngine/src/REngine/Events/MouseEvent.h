#pragma once

#include "Event.h"

namespace REngine
{
    class RE_API MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_mouseX(x), m_mouseY(y)
        {
        }

        // Const methods prevent changing values of data in their own class
        inline float GetX() const { return m_mouseX; }
        inline float GetY() const { return m_mouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved);
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_mouseX;
        float m_mouseY;
    };

    class RE_API MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_xOffset(xOffset), m_yOffset(yOffset)
        {
        }

        inline float GetXOffset() const { return m_xOffset; }
        inline float GetYOffset() const { return m_yOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_xOffset;
        float m_yOffset;
    };

    class RE_API MouseButtonEvent : public Event
    {
    public:
        inline int GetMouseButton() const { return m_button; }
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        MouseButtonEvent(int button) 
            : m_button(button) {}

        int m_button;

    };

    class RE_API MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class RE_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)

    };
}