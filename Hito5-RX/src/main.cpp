#include <Arduino.h>
#include <Wire.h>

// put function declarations here:
void onReceiveEvent(int q);

void setup()
{
  // put your setup code here, to run once:
  Wire.begin(8); //Unirse al bus con la direccion 8
  Wire.onReceive(onReceiveEvent);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void onReceiveEvent(int q)
{
  char c =  Wire.read();
  if (c == '0')
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (c == '1')
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}