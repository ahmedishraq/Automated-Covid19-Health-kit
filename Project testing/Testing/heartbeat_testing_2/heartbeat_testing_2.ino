int sensorPin = 0;
void setup() {
   Serial.begin(9600);
}
void loop ()
{
   while(1)
   {
     Serial.print(analogRead(sensorPin));
     Serial.print('\n');
     delay(100);
   }
} 
