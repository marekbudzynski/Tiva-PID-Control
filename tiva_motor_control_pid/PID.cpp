#include "PID.h"

#include <Energia.h> 

PID::PID(float target_rpm, float _Kp, float _Ki, float _Kd) {
  p_current_rpm = 0.0f;
  p_output = 0.0f;
  p_target_rpm = target_rpm;
  PID::set_tunings(_Kp, _Ki, _Kd);
}

/*
  Kp = Kp
  Ki = Ki * dt
  Kd = Kd / dt
*/
void PID::update(double dt) {

    float u_target_rpm = p_target_rpm;
    float u_current_rpm = p_current_rpm;

    // calculate error
    float error = u_target_rpm - u_current_rpm;

    Serial.println(error);
    float d_input = u_current_rpm - last_rpm;

    // add proportional term
    float output = Kp * error;

    // calculate integral total
    integral_total += Ki * error * dt;

    // add integral term
    output += integral_total;

    // add derivative term
    output -= ((Kd * d_input) / dt);

    // cap output
    if(output > max_out) { output = max_out; } else if(output < 0.0f) { output = 0.0f; }

    // allows fast decay to zero, if target rpm is 0
    /*
    if(*p_target_rpm==0.0f && (output>0.0f || output<0.0f)) {
      reset_pid();
      *p_output = 0.0f;
    } else {
      *p_output = output;
    }*/

    int pwm = (int) output;

    analogWrite(PB_6, pwm);
    //Serial.println(pwm);

    last_rpm = u_current_rpm;

}

void PID::set_tunings(float _Kp, float _Ki, float _Kd) {
  if(_Kp<0 || _Ki<0 || _Kd<0) { return; }
  Kp = _Kp;
  Ki = _Ki;
  Kd = _Kd;
}

void PID::reset_pid() {
  integral_total = 0.0f;
}

float PID::get_Kp() {
  return Kp;
}

float PID::get_Ki() {
  return Ki;
}

float PID::get_Kd() {
  return Kd;
}
