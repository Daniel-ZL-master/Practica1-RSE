#include <Arduino.h>
#include <Wire.h>

int estado = 0;
char str[100];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); // Sin dirección, somos writer
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (estado)
  {
  case 0:
    Serial.println("Introduce el valor del led con 0 o 1");
    estado = 1;
    break;
  case 1:
    int i = 0;
    while (Serial.available() > 0)
    {
      str[i++] = Serial.read();
      if (i >= 100)
      {
        i = i-1;
      }
    }
    str[i] = '\0';
    if (i > 0) // cambiamos de estado en caso de haber leido algo
    {
      estado = 2;
    }
    break;
  case 2:
    Wire.beginTransmission(8);
    Wire.write(str[0]);
    Wire.endTransmission();
    estado = 0;
    break;
  default:
    break;
  }
}