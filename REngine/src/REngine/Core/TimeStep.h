#pragma once
namespace REngine
{
    class TimeStep
    {
    public:
        TimeStep(float time = 0.0f)
            : m_time(time)
        {
        }

        operator float() const { return m_time; }

        inline float GetSeconds() const { return m_time; }
        inline float GetMilliseconds() const { return m_time * 1000.0f; }

    private:
        float m_time;
    };
}

