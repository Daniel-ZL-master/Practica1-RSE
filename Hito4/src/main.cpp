#include <Arduino.h>
#include <string.h>
#include "BBTimer.hpp"

// put variables here:
String cmd;
int raw_adc;
int seconds;
bool imprimirADC;
float duty;
unsigned long tprev_ON;
unsigned long tprev_OFF;
int t_ON;
int t_OFF;
int estado = 0;

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
  

  unsigned long tactual = micros();
  t_ON = duty * 200;
  t_OFF = 200 - t_ON;
  if (t_ON > 0)
  {
    switch (estado)
    {
    case 0:
      digitalWrite(2, HIGH);
      tprev_ON = tactual;
      estado = 1;
      break;
    case 1:
      if (tactual - tprev_ON >= t_ON)
      {
        estado = 2;
      }
      break;
    case 2:
      digitalWrite(2, LOW);
      tprev_OFF = tactual;
      estado = 3;
      break;
    case 3:
      if (tactual - tprev_OFF >= t_OFF)
      {
        estado = 0;
      }
      break;
    }
  }
  else
  {
    digitalWrite(2, LOW);
  }
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