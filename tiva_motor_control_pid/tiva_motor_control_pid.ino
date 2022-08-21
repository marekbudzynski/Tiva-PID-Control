
#include <Energia.h> 
#include "TivaQEI.h"

/** encoder variables */
int16_t vel;
float current_rps;

// encoders
Encoder encoder_a;

// pid parameters
float kP = 6.4f;
float kI = 5.8f;
float kD = 0.0f;

float target_rps = 104.0f;
float output = 0.0f;
int pwm_scaled = 0;

const int16_t min_out = 0;
const int16_t max_out = 255;

void setup() {
  
  Serial.begin(115200);

  // QEI0_A = PD_6
  encoder_a.init_encoder(QEI0_BASE);

  // set PB_6 as output
  pinMode(PB_6, OUTPUT);

  // 0 to 255
  analogWrite(PB_6, 0);

}

long previousMillis = 0; 

void loop() {

  unsigned long currentMillis = millis();
  long delta_t = currentMillis - previousMillis;
  double dt = delta_t / 1000.0d;
    
  if(delta_t > 50) {

    update_pid(dt);
    
    previousMillis = currentMillis;

 }

}

float proportional = 0.0f;
float integral = 0.0f;
float derivative = 0.0f;

float last_error = 0.0f;
float error_integral = 0.0f;

void update_pid(double dt) {

  // encoder velocity
  vel = encoder_a.get_velocity();
  
  // current rps
  current_rps = vel * 0.833333333f;

  // calculate error
  float error = target_rps - current_rps;

  // calculate error derivative
  float error_derivative = (error - last_error) / dt;

  // calculate error integral
  error_integral += error * dt;

  // cap error integral
  if(error_integral > max_out) { error_integral = max_out; } else if(error_integral < 0) { error_integral = 0.0f; }

  proportional = kP * error;
  integral = kI * error_integral;
  derivative = kD * error_derivative;

  output = proportional + integral + derivative;

  pwm_scaled = (int) output;

  // cap output
  if(pwm_scaled > max_out) { pwm_scaled = max_out; } else if(pwm_scaled < min_out) { pwm_scaled = min_out; }
  
  //Serial.print(pwm_scaled);
  //Serial.print('\t');

  Serial.print(current_rps);
  Serial.print('\t');
  Serial.println(target_rps);

  analogWrite(PB_6, pwm_scaled);

  last_error = error;
  

}
