#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);

// ****** heartbeat initializing start *****
int sensorPin = 0; // Analog Pin where the S is connected to
int period = 20; // delay 20 ms
long count = 0; // count for number of picks
long HrtRate =0; // Heart rate (determined)
long Cal_hrt_rate = 0; // calibrated heart rate

// For calculation of the difference of time
long time1 = 0; 
long time2 = 0;

// ****** heartbeat initializing end ******

// ****** pir motion sensor intializing start *****
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 8;    //the digital pin connected to the PIR sensor's output
int ledPin = 9;    //the digital pin connected to the LED output
int Buzzer = 10;    //the digital pin connected to the BUZZER output


// ****** pir motion sensor intializing end *****

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("CSE360 Project");
  delay(5000);
  // ** setup for pir motion sensor start **
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(pirPin, LOW);

    //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  lcd.clear();
  lcd.print("calibrating");
  lcd.setCursor(0,1);
  lcd.print("sensor....");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    lcd.clear();
    lcd.print("Calibration");
    lcd.setCursor(0,1);
    lcd.print("Done!");
    delay(5000);
    Serial.println("SENSOR ACTIVE");
    lcd.clear();
    lcd.print("SENSOR ACTIVE");
    delay(2000);
  // ** setup for pir motion sensor end **
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
    heartBeat();
}


// heartbeat measuring method
void heartBeat(){
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
    if(HrtRate > 150){
    digitalWrite(ledPin,HIGH);
    tone(Buzzer,500);  
    lcd.clear();
    lcd.home (); // set cursor to 0,0
    lcd.print ("HEART RATE=");
    lcd.setCursor (0,1);        // go to start of 2nd line
    lcd.print(Cal_hrt_rate);
    lcd.setCursor (3,1);
    lcd.print ("Pulse/min");
    delay (5000); // after 5 sec
    //} 
    }
    else{
      Serial.println("Not working");
      lcd.clear();
      lcd.print("HEHE!");
      }     
    
  }  
}
