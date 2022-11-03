#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IR A0
#define PIN_SERVO 10
#define PIN_LED 9

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 30   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5
Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist_ema;
void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float dist;
  float dist_prev=100.0;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  
  // Read IR Sensor value !!!
  a_value=analogRead(PIN_IR);
  // Convert IR sensor value into distance !!!
  dist=6762.0/((a_value-9)-4.0)*10.0 - 60;
  // we need distance range filter here !!!
  if (dist<=100.0){
    dist = dist_prev;
    digitalWrite(PIN_LED,0);
  }else if (dist>=250.0) {
    dist = dist_prev;    // Set Higher Value
    digitalWrite(PIN_LED, 0);       // LED OFF
  } else {    // In desired Range   // LED ON 
    digitalWrite(PIN_LED,1);
    if (dist_ema<=100.0){
      myservo.writeMicroseconds(_DUTY_MIN);
    }
    else if (dist_ema<=250.0){
      myservo.writeMicroseconds(1846/180*(dist_ema-100)+553);
    }
    else{
      myservo.writeMicroseconds(_DUTY_MAX);
    }
  }
  // we need EMA filter here !!!
  dist_ema = dist*_EMA_ALPHA + (1-_EMA_ALPHA)*dist_ema;

  // map distance into duty
  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("ADC Read: "); Serial.print(a_value);
  Serial.print(" = ");
  Serial.print((a_value / 1024.0) * 5.0);
  Serial.print(" Volt => Duty : ");
  Serial.print(duty);
  Serial.println("usec");
}
