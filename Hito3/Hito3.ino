#include <Arduino.h>
#include <mbed.h> // Necesario para acceder a las funciones nativas del nRF52

// Creamos un objeto PwmOut.
mbed::PwmOut pwm_pin(digitalPinToPinName(2));

void setup() {
  Serial.begin(9600);
  // T = 1 / 5000 = 0.0002 segundos
  pwm_pin.period(0.0002f); 
  
  // Inicializamos en 0
  pwm_pin.write(0.0f);
}

void loop() {
  int raw_adc = analogRead(A0);

  float duty = raw_adc / 1023.0;

  pwm_pin.write(duty);
  delay(10);
}
