#include <Arduino.h>
#include <string.h>
#include "BBTimer.hpp"
#include <mbed.h> // Necesario para acceder a las funciones nativas del nRF52

// put variables here:
String cmd;
int raw_adc;
int seconds;
bool imprimirADC;
float duty;
// Creamos un objeto PwmOut.
mbed::PwmOut pwm_pin(digitalPinToPinName(2));

BBTimer my_t0(BB_TIMER0);

// put function declarations here:
void shell();

void t0CallBack()
{
  imprimirADC = true;
  my_t0.updatePeriod(seconds*1000000);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  my_t0.setupTimer(1000,t0CallBack);
  pwm_pin.period(0.0002f); 
  pwm_pin.write(0.0f);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    cmd = Serial.readString();
    shell();
  }

  if (imprimirADC)
  {
    raw_adc = analogRead(A0);
    Serial.print("LECTURA ADC: ");
    Serial.println(raw_adc);
    imprimirADC=false;
  }
  raw_adc = analogRead(A0);
  pwm_pin.write(duty);
  delay(10);
}

// put function definitions here:
void shell()
{
  Serial.print("COMANDO: ");
  Serial.println(cmd);
  if (cmd == "ADC")
  {
    raw_adc = analogRead(A0);
    Serial.print("LECTURA ADC: ");
    Serial.println(raw_adc);
  }
  else if (cmd.substring(0, cmd.indexOf("(")) == "ADC")
  {
    String sub = cmd.substring(cmd.indexOf("(") + 1, cmd.indexOf(")"));
    seconds = sub.toInt();
    // SUBRUTINA DE INTERRUPCIONES

    if (seconds == 0)
    {
      my_t0.timerStop();
    }
    else
    {
      my_t0.timerStart();
    }
  }
  else if (cmd.substring(0, cmd.indexOf("(")) == "PWM")
  {
    String sub = cmd.substring(cmd.indexOf("(") + 1, cmd.indexOf(")"));
    int number = sub.toInt();
    duty = (float)number / 10.0;
  }
  else
  {
    Serial.println("COMANDO NO RECONOCIDO");
  }
}