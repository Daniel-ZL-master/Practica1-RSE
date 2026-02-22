#include <Arduino.h>
#include <Wire.h>

#define DIRECCION_I2C 8
#define SDA_PIN 8 
#define SCL_PIN 9 

struct __attribute__((packed)) PaqueteDatos {
  byte id_paquete;
  float valores[5];
};

PaqueteDatos paqueteRecibido;
float datosCompletos[15];

volatile bool nuevosDatosListos = false;

// Variables para controlar el tiempo del LED sin bloquear el código
unsigned long tiempoInicioLed = 0;
bool ledEstaEncendido = false;

void eventoRecepcion(int cuantosBytes) {
  if (cuantosBytes >= sizeof(paqueteRecibido)) {
    Wire.readBytes((byte*)&paqueteRecibido, sizeof(paqueteRecibido));
    byte id = paqueteRecibido.id_paquete;
    if (id >= 0 && id < 3) {
      for(int i = 0; i < 5; i++) {
        datosCompletos[(id * 5) + i] = paqueteRecibido.valores[i];
      }
      if (id == 2) {
        nuevosDatosListos = true;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Wire.setPins(SDA_PIN,SCL_PIN);
  Wire.begin(DIRECCION_I2C);
  Wire.onReceive(eventoRecepcion);
  
  Serial.println("ESP32-S3 listo y usando millis() para no bloquearse...");
}

void loop() {
  if (nuevosDatosListos) {
    // Apagamos la bandera INMEDIATAMENTE para no perder futuros datos
    nuevosDatosListos = false; 
    
    // Mostramos los datos
    Serial.println("==================================");
    Serial.println("NUEVA CAPTURA DE IMU RECIBIDA:");
    for (int muestra = 0; muestra < 5; muestra++) {
      Serial.print("Muestra "); Serial.print(muestra + 1); Serial.print(": \t");
      Serial.print(datosCompletos[(muestra * 3) + 0], 3); Serial.print("\t");
      Serial.print(datosCompletos[(muestra * 3) + 1], 3); Serial.print("\t");
      Serial.println(datosCompletos[(muestra * 3) + 2], 3);
    }
    Serial.println("==================================\n");
    
    // Encendemos el LED y guardamos en qué milisegundo exacto lo encendimos
    digitalWrite(LED_BUILTIN, HIGH);
    tiempoInicioLed = millis();
    ledEstaEncendido = true;
  }

  if (ledEstaEncendido && (millis() - tiempoInicioLed >= 1000)) {
    digitalWrite(LED_BUILTIN, LOW);   // Lo apagamos
    ledEstaEncendido = false;     // Actualizamos el estado
  }
}