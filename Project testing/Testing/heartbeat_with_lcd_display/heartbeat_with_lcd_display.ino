/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(2,3,4,5,6,7);
int led = 8;

// heartbeat code
int sensorPin = 0; // Analog Pin where the S is connected to
int period = 20; // delay 20 ms
long count = 0; // count for number of picks
long HrtRate =0; // Heart rate (determined)
long Cal_hrt_rate = 0; // calibrated heart rate

// For calculation of the difference of time
long time1 = 0; 
long time2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("CSE360 Project");
}

//void loop() {
//  // set the cursor to column 0, line 1
//  // (note: line 1 is the second row, since counting begins with 0):
//  if()
//  lcd.setCursor(0, 1);
//  // print the number of seconds since reset:
//  lcd.print(millis() / 1000);
//}

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
  Serial.print(count);
  Serial.print("\n");
  if (count>49) // when 50 picks arrived
  {
    time2 = millis(); // time after the specific number of picks
    HrtRate = 30000*count/(time2 - time1); // Heart rate at 1 min =  (count/2)*60000 ms,1 pick = count/2
    Cal_hrt_rate = 0.206*HrtRate-16.09; // CALIBRATED EQUATION obtained from the experiment on different volunteers.
    //if ((Cal_hrt_rate>59) && (Cal_hrt_rate<110)) // considered the max and min heart rate could be 90 and 60
    //{
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
    //}      
    
  }
  }
