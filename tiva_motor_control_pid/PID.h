#ifndef PID_h
#define PID_h

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

class PID {

  public:

    PID(float target_rpm, float _Kp, float _Ki, float _Kd);

    void set_tunings(float _Kp, float _Ki, float _Kd);

    void update(double dt);

    void reset_pid();

    float get_Kp();
    float get_Ki();
    float get_Kd();

  private:

    const int16_t min_out = 0;
    const int16_t max_out = 255;

    float Kp;
    float Ki;
    float Kd;

    float p_current_rpm;
    float p_target_rpm;
    float p_output;

    float last_rpm = 0.0;
    float integral_total = 0.0f;

};

#ifdef __cplusplus
}
#endif

#endif
