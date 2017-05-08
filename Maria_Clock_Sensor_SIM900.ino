#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

//define pin for proximity sensor
const int trigPin = 6; 
const int echoPin = 5;

// defines variables for proximity sensor
float duration;
float distance;
float average_distance; 
float sum_distance;

// defines variables for time routine
unsigned long timeNow = 0;
unsigned long timeLast = 0;

// Time start Settings:
int startingHour = 16; // custom time 
                       // The clock will start running at startimehour but nothing else would happen until time set below
int seconds = 00;
int minutes = 00;
int hours = startingHour;
int days = 0;

// ------------------------------------------------------
void setup() {   // put your setup code here, to run once
// ------------------------------------------------------
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
delay(500);
Serial.begin(9600);
//Serial.begin(19200);
SIM900.begin(19200);
}
// ----------------------------------------------------------
void loop() {  // put your main code here, to run repeatedly
// ----------------------------------------------------------

// clock running section
// =====================
timeNow = millis()/1000; // the number of milliseconds that have passed since boot
seconds = timeNow - timeLast; //the number of seconds that have passed since the last time 60 seconds was reached
if (seconds == 60) {  //if one minute has passed, start counting milliseconds from zero again and add one minute to the clock
  timeLast = timeNow;
  minutes = minutes + 1;
}
if (minutes == 60){  // if one hour has passed, start counting minutes from zero and add one hour to the clock
  minutes = 0;
  hours = hours + 1;
}
if (hours == 24){ // if 24 hours have passed , add one day
  hours = 0;
  // days = days + 1; // irrelevant for daily reading the level
  }
          // check if the clock is running - DEBUGGING ONLY. Comment out after testing is finished!
          //Serial.print(hours);
          //Serial.print(":");
          //Serial.print(minutes);
          //Serial.print(":"); 
          //Serial.println(seconds);
          // end debugging section

// Daily sensor reading at 16:01:00
// ===================================
if(hours == 16 && minutes == 05 && seconds == 00) { // for demo set this to 16:01:00
  int duration = 0.0;
  int distance = 0.0;
  int sum_distance = 0.0;
  int average_distance = 0.0;
  for (int i=1; i <= 26; i++){  // Reads 25 sensor values every day when the clock hits 16:01.00
    digitalWrite(trigPin, LOW);  // Clears the trigPin
    delayMicroseconds(5); // Sets the trigPin on HIGH state for 2 micro seconds
    digitalWrite(trigPin, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
    distance = duration*0.034/2.0;  // Calculating the distance
    sum_distance = sum_distance + distance;
    average_distance = sum_distance / 25.0;
    }
          //print to the serial monitor - DEBUGGING ONLY. Comment out after testing is finished!
          //Serial.println(distance);
          //Serial.println(sum_distance);
          //Serial.println(average_distance);
          // end debugging section
    
    // Power up the GSM shield and send SMS ONLY if condition reached
    // ==============================================================
    if(average_distance > 10) { 
    
        //digitalWrite(9, HIGH);  // software equivalent of pressing the GSM shield "power" button - DISABLED
        //delay(1000);            // because on this shield it requires soldering over resistor R13 
        //digitalWrite(9, LOW);
        //delay(5000);
      
      SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
      delay(100);
      SIM900.println("AT + CMGS = \"+447502352760\""); // recipient's mobile number, in international format
      delay(100);
      SIM900.println("You're running low on Rice. Rice has been added to your shopping list"); // message to send
      delay(100);
      SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
      delay(100);
      SIM900.println();
      delay(5000); // give module time to send SMS

      SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
      delay(100);
      SIM900.println("AT + CMGS = \"+14157280914\""); // recipient's mobile number, in international format
      delay(100);
      SIM900.println("#SmartPantry"); // message to send
      delay(100);
      SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
      delay(100);
      SIM900.println();
      delay(5000); // give module time to send SMS
      
        //digitalWrite(9, HIGH);  // software equivalent of pressing the GSM shield "power" button - DISABLED
        //delay(1000);            // because on this shield it requires soldering over resistor R13 
        //digitalWrite(9, LOW);
        //delay(5000);
    }

  // delay(1000); // ensures that only a set of 25 measurements are done
}
          // check if the clock still running - DEBUGGING ONLY. Comment out after testing is finished!
          //Serial.print(hours);
          //Serial.print(":");
          //Serial.print(minutes);
          //Serial.print(":"); 
          //Serial.println(seconds);
          // end debugging section
            
// end of loop

}
