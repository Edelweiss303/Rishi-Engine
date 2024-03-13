#pragma once
#include "REpch.h"
#include "REngine/Core.h"

namespace REngine
{
    // Events are currently blocking
    // When an event occurs it immediately gets dispatched and must be dealt with right away

    // A better strategy might be to buffer events in an event bus and process them during the event part of an update stage

    enum class EventType
    {
        None = 0,

        WindowClose,
        WindowResize, 
        WindowFocus, 
        WindowLostFocus, 
        WindowMoved, 

        AppTick, 
        AppUpdate, 
        AppRender,

        KeyPressed, 
        KeyReleased,
        KeyTyped,

        MouseButtonPressed, 
        MouseButtonReleased, 
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication  = BIT(0),
        EventCategoryInput        = BIT(1),
        EventCategoryKeyboard     = BIT(2),
        EventCategoryMouse        = BIT(3),
        EventCategoryMouseButton  = BIT(4)
    };

// macros to define common virtual function implementations
// these are so handy god damn

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class RE_API Event
    {
        friend class EventDispatcher;

    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool isInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

    public:
        bool Handled = false;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event)
            : m_event(event)
        {
        }

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            // if the event type we're trying to handle matches the template type, run the function
            if (m_event.GetEventType() == T::GetStaticType())
            {
                m_event.Handled = func(*(T*)&m_event);
                return true; 
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}