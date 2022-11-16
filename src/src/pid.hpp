#ifndef PID_H
#define PID_H

#include "mbed.h"

class PID
{
private:
    std::chrono::milliseconds last_time{};
    double kp{}, kd{}, ki{}; // gains

    double delta_time{}, setpoint{}, measurement{}, output{}, cumulative_err{}, last_err{}, error{};
    Timer _timer{};

protected:
public:
    PID(double kp_, double kd_, double ki_) : kp{kp_}, kd{kd_}, ki{ki_}
    {
        _timer.start();
    }
    // TODO: use ticker instead of timer, calculate updates every few ms
    ~PID();

    // update PID
    double update(double &setpoint, double &measurement)
    {
        // capture timestamp since last calc
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time());
        delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(time - last_time).count();
        // delta_time = (double)(time - last_time);
        printf("now :%lld delta: %lld, last: %lld  \n", time.count(), delta_time, last_time.count());
        // calculate error
        error = setpoint - measurement;
        // calculate integral error
        cumulative_err += (error * delta_time);

        // calculate derivative error
        double deriv_err = (error - last_err) / delta_time;

        last_err = error;
        last_time = time;

        // calculate output
        output = kp * error + ki * cumulative_err + kd * deriv_err;
        printf(" set: %.3f, in: %.3f , error: %.3f, cum: %.3f, deriv : %.3f, out: %.3f  \n", setpoint, measurement, error, cumulative_err, deriv_err, output);
        return output;
    }
};

#endif
