#include<Servo.h>
Servo Myservo;
int pos;
void setup()
{
Myservo.attach(12);
}

void loop()
{
  
  
for(pos=0;pos<=90;pos++){
Myservo.write(pos);
delay(20);
}
  delay(1000);
  
//  for(pos=90;pos>=0;pos--){
//Myservo.write(pos);
//delay(5);
//}
//  delay(1000);
  
}
