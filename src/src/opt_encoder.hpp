#ifndef TEST_INT_H
#define TEST_INT_H

#include "mbed.h"

class OptEncoder
{
private:
    InterruptIn _interrupt_pin;
    volatile uint32_t _count{};
    uint32_t _rpm{};
    Timer _timer{};

public:
    // test documentation
    OptEncoder(PinName enc_pin) : _interrupt_pin(enc_pin)
    {
        _interrupt_pin.rise(callback(this, &OptEncoder::rise_isr));
    }

    ~OptEncoder();

    void rise_isr()
    {
        _count++;
    }

    int read()
    {
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time()).count();
        if (time == 0)
        {
            _timer.start();
            return 0;
        }
        _rpm = (6000 * _count) / time;
        _count = 0;

        _timer.reset();
        return _rpm;
    }
};

#endif
