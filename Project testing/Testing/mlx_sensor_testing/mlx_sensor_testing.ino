#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp_amb;
float temp_obj;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Temperature Sensor MLX90614");
  mlx.begin();
}

void loop() 
{
//  float temperature = mlx.readObjectTempF();
//  Serial.println(temperature);
//  delay(500);
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC();

  if(temp_obj > 37){
    Serial.println("HIGH TEMP..");
    lcd.clear();
    lcd.print("HIGH TEMP...");
    delay(700);  
  }
  else{
  }

  // lcd display
  lcd.setCursor(0,0);
  lcd.print("Room Temp:");
  lcd.setCursor(10,0);
  lcd.print(temp_amb);
  lcd.setCursor(15,0);
  lcd.write(1);
  lcd.setCursor(0,1);
  lcd.print("Body Temp:");
  lcd.setCursor(10,1);
  lcd.print(temp_obj);
  lcd.setCursor(15,1);
  lcd.write(1);

  // Serial monitor
  Serial.print("Room Temp:");
  Serial.println(temp_amb);
  Serial.print("Body Temp:");
  Serial.println(temp_obj);

  delay(1000);
}
