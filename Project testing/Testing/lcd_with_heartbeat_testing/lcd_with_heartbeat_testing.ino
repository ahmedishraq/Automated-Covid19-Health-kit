

// Attempt for calibration of KY039
// Sensor pin S connect to Arduino pin Analoog 1 / A1
// Sensor pin + (middle pin) connect to Arduino pin 5+
// Sensor pin - connect to Arduino pin GND
#include <math.h>
#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>

int sensorPin = 1; // Analog Pin where the S is connected to
int period = 20; // delay 20 ms
long count = 0; // count for number of picks
long HrtRate =0; // Heart rate (determined)
long Cal_hrt_rate = 0; // calibrated heart rate

// For calculation of the difference of time
long time1 = 0; 
long time2 = 0;

LiquidCrystal_I2C  lcd(0x27,20,4); // 0x3F is the I2C bus address for an unmodified backpack


void setup() {
  Serial.begin(9600);
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  //lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
}

void loop() {
  int data1 = analogRead(sensorPin);
  delay (period);
  int data2 = analogRead(sensorPin);
  int diff = data2 - data1; // difference of two simultanious values

  if (diff>0) // graph is moving upwards 
  {
    while (diff>0)
    {
    data1 = analogRead(sensorPin);
    delay (period);
    data2 = analogRead(sensorPin);
    diff = data2 - data1;
    } // coming out of the while loop means the graph has started moving downward
    count++; 
    if (count == 1) 
    {
    time1 = millis(); // time at the time of first pick
    }
  }
  else // graph is moving upwards 
  {
    while (diff<=0)
    {
    data1 = analogRead(sensorPin);
    delay (period);
    data2 = analogRead(sensorPin);
    diff = data2 - data1;
    } // coming out of the while loop means the graph has started moving downwardward
    
    count++; // pick counted
    if (count == 1)
    {
    time1 = millis(); // time at the time of first pick
    }
  }
  if (count>49) // when 50 picks arrived
  {
    time2 = millis(); // time after the specific number of picks
    HrtRate = 30000*count/(time2 - time1); // Heart rate at 1 min =  (count/2)*60000 ms,1 pick = count/2
    Cal_hrt_rate = 0.206*HrtRate-16.09; // CALIBRATED EQUATION obtained from the experiment on different volunteers.
    if ((Cal_hrt_rate>59) && (Cal_hrt_rate<91)) // considered the max and min heart rate could be 90 and 60
    {
    Serial.print("Heart Rate= ");
    Serial.println(HrtRate);
    count  = 0; // For new loop and subsiquent calculations
    lcd.home (); // set cursor to 0,0
    lcd.print ("HEART RATE=");
    lcd.setCursor (0,1);        // go to start of 2nd line
    lcd.print(Cal_hrt_rate);
    lcd.setCursor (3,1);
    lcd.print ("Pulse/min");
    delay (5000); // after 5 sec
    }      
    
  }
  }
