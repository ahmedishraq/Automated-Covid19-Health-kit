#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(2,3,4,5,6,7);

// ****** heartbeat initializing start *****
int sensorPin = 0; // Analog Pin where the S is connected to
int period = 20; // delay 20 ms
long count = 0; // count for number of picks
long HrtRate =0; // Heart rate (determined)
long Cal_hrt_rate = 0; // calibrated heart rate
boolean hb = false;

#define samp_siz 4
#define rise_threshold 5

Servo myservo;

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

int motion_counter = 0;
int count_test = 0;

int pirPin = 8;    //the digital pin connected to the PIR sensor's output
int ledPin = 9;    //the digital pin connected to the LED output
int Buzzer = 10;    //the digital pin connected to the BUZZER output
int ledPin2 = 13;

// ****** pir motion sensor intializing end *****

// *** mlx90614 temperature sensor initiallizing start ***
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp_amb;
float temp_obj;

// *** mlx90614 temperature sensor initiallizing end ***

void setup() {
  Serial.begin(9600);
  myservo.attach(11);
  myservo.write(0);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("CSE360 Project");
  delay(5000);
  // ** setup for pir motion sensor start **
 // pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  //digitalWrite(pirPin, LOW);

    //give the sensor some time to calibrate
  Serial.print("Calibrating Sensor ");
  lcd.clear();
  lcd.print("calibrating");
  lcd.setCursor(0,1);
  lcd.print("sensor....");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(10);
      digitalWrite(ledPin,HIGH);
      delay(100);
      digitalWrite(ledPin,LOW);
      delay(100);
      }
    Serial.println();
    Serial.println("Calibration Done!");
    lcd.clear();
    lcd.print("Calibration");
    lcd.setCursor(0,1);
    lcd.print("Done!");
    delay(5000);
    mlx.begin();
    Serial.println("SENSOR ACTIVE");
    lcd.clear();
    lcd.print("SENSOR ACTIVE");
    delay(2000);
  // ** setup for pir motion sensor end **
}

void loop() {
  temperature(); 
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
    lcd.clear();
    lcd.home (); // set cursor to 0,0
    lcd.print ("HEART RATE=");
    lcd.setCursor (0,1);        // go to start of 2nd line
    lcd.print(HrtRate);
    lcd.setCursor (3,1);
    lcd.print ("Pulse/min");
    delay (7000); // after 7 sec
    //}      
  }  
}

// pir motion detection method
void motionDetect(){
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       tone(Buzzer,500);
       motion_counter = motion_counter + 1;
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
       noTone(Buzzer);
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }     
}

// MLX90614 Sensor
void temperature(){
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempF();

  if(temp_obj > 90){
    Serial.print("Body Temp:");
    Serial.println(temp_obj);
    Serial.println("HIGH TEMPERATURE");
    Serial.println("Check hartbeat");
    digitalWrite(ledPin,HIGH);
    tone(Buzzer,500);
    delay(2500);
    digitalWrite(ledPin, LOW);
    noTone(Buzzer);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Body Temp:");
    lcd.setCursor(10,0);
    lcd.print(temp_obj);
    delay(3000);
    lcd.clear();
    lcd.home();
    lcd.print("HIGH TEMPERATURE");
    lcd.setCursor(0,1);
    lcd.print("Check heartbeat");
    delay(5000);
    for(int i=0;i<50;i++){
      heartBeat();
      lcd.clear();
      lcd.print("Measuring");
      lcd.setCursor(0,1);
      lcd.print("Heart Beat...");
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin,LOW);
      delay(100);  
    }
  }
  else if(temp_obj > 87 && temp_obj < 89){
  // lcd display
  lcd.setCursor(0,0);
  lcd.print("Room Temp:");
  lcd.setCursor(10,0);
  lcd.print(temp_amb);
  lcd.setCursor(15,0);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Body Temp:");
  lcd.setCursor(10,1);
  lcd.print(temp_obj);
  //delay(5000);
  lcd.setCursor(15,1);
  lcd.print("F");
  digitalWrite(ledPin2,HIGH);
  myservo.write(90);
  
  // Serial monitor
  Serial.print("Room Temp:");
  Serial.println(temp_amb);
  Serial.print("Body Temp:");
  Serial.println(temp_obj);

  delay(4000);
  digitalWrite(ledPin2,LOW);
  myservo.write(0);
  }
  else{
    
  // lcd display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mini Health Kit");
  //lcd.setCursor(10,0);
  //lcd.print(temp_amb);
  //lcd.setCursor(15,0);
  //lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Room Temp:");
  lcd.setCursor(10,1);
  lcd.print(temp_amb);
  //delay(5000);
  lcd.setCursor(15,1);
  lcd.print("C");

  
  // Serial monitor
  Serial.println("Mini Health Kit");
  //Serial.println(temp_amb);
  Serial.print("Room Temp:");
  Serial.println(temp_amb);

  delay(4000);
  }
  
}
