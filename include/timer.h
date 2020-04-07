#include <Arduino.h>

class Timer
{
public:

    void setInterval(uint32_t interval);

    bool available();

    void update();

private:

    uint32_t current;
    uint32_t interval;
};

inline void Timer::setInterval(uint32_t value)
{
    interval = value;
}

inline bool Timer::available()
{
    return millis() - current >= interval;
}

inline void Timer::update()
{
    current = millis();
}