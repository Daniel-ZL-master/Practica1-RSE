#include <Arduino.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h>

// Estructura del paquete de I2C (21 bytes en total)
struct __attribute__((packed)) PaqueteDatos {
  byte id_paquete;
  float valores[5];
};

PaqueteDatos paquete;
float datosIMU[15]; // Arreglo para almacenar: 5 muestras x 3 ejes (X, Y, Z)

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Iniciar I2C como Maestro
  
  // Esperar a que el puerto serie se abra (opcional, útil para depurar)
  while (!Serial);
  
  // Iniciar la IMU
  if (!IMU.begin()) {
    Serial.println("Error al iniciar la IMU!");
    while (1);
  }
  
  Serial.println("Nano 33 BLE Sense listo.");
  Serial.println("Envía cualquier letra por el Monitor Serie para capturar 1 segundo de datos...");
}

void loop() {
  if (Serial.available() > 0) {
    while(Serial.available() > 0) Serial.read(); 

    Serial.println("Capturando datos (1 segundo)...");
    
    // CAMBIAR MUESTREO A INTERRUPT 
    for (int i = 0; i < 5; i++) {
      float x, y, z;
      
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
      }
      
      // Guardar en arreglo de 15 posiciones
      datosIMU[(i * 3) + 0] = x;
      datosIMU[(i * 3) + 1] = y;
      datosIMU[(i * 3) + 2] = z;
      
      delay(200);
    }
    
    Serial.println("Captura terminada. Enviando por I2C al ESP32...");
    
    // Enviar los 15 floats divididos en 3 paquetes de 5 floats
    for (byte id = 0; id < 3; id++) {
      paquete.id_paquete = id;
      
      for (int j = 0; j < 5; j++) {
        paquete.valores[j] = datosIMU[(id * 5) + j];
      }
      
      Wire.beginTransmission(8); // Dirección del ESP32-S3
      Wire.write((byte*)&paquete, sizeof(paquete));
      Wire.endTransmission();
      
      delay(10);
    }
    
    Serial.println("Envío completado.\nEnvía otra letra para repetir.");
  }
}