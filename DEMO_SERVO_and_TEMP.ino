#include <MicroView.h>      // include MicroView library

MicroViewWidget *widget;    // declare widget pointer

int sensorPin = A0;     // select the input pin for the temperature sensor
int sensorValue = 0;    // variable to store the value coming from the sensor
int FAN_SPEED = 5;
float TEMP_MAX = 27.0;
int SPIN_TOTAL = 5;
int PAUSE_TIME = 1000;
#include <Servo.h>

Servo myservo;
int pos = 0;
int spinCount = -1;

void setup() {
    myservo.attach(6);
    pinMode(sensorPin,INPUT);   // set sensor pin as INPUT
    uView.begin();              // start MicroView
    uView.clear(PAGE);          // clear page
    widget = new MicroViewGauge(32,24,0,255,WIDGETSTYLE1);      // declare as gauge widget
    uView.drawChar(47,33,67);   // Character C is ASCII code 67
}

void loop() {
    sensorValue= analogRead(sensorPin);             // read sensor pin value
    float voltage = sensorValue * 5.0;              // voltage at pin in volt
    voltage /= 1024.0;                              // voltage = sensorValue x (5/1024)
    float temperatureC = (voltage - 0.5) * 100 ;    // C = (voltage - 0.5) x 100
    widget->setValue(temperatureC);                 // set temperature value to the gauge

    uView.display();
    if((temperatureC > TEMP_MAX) && (spinCount == -1)) {
      spinCount = SPIN_TOTAL;
    } else if(spinCount == 0) {
      spinCount = -1;
      delay(1000);
    } else if(spinCount > 0) {
          spinCount = spinCount - 1;
          for (pos = 0; pos <= 180; pos += FAN_SPEED) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          }
          for (pos = 180; pos >= 0; pos -= FAN_SPEED) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          }
    }
}
