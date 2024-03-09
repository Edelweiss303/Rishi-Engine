#pragma once

#include "Event.h"

namespace REngine
{
    class RE_API KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return m_keycode; }
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(int keycode) 
            : m_keycode(keycode) 
        {
        }

        int m_keycode;

    };

    class RE_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent(keycode), m_repeatCount(repeatCount) 
        {
        }

        inline int GetRepeatCount() const { return m_repeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keycode << " (" << m_repeatCount << ") repeats";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_repeatCount;
    };

    class RE_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent(keycode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)

    };
}