#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include <BBTimer.hpp>

float x[20], y[20], z[20];
float xG[20],yG[20],zG[20];
float xM[20], yM[20],zM[20];
int i=0;

BBTimer my_t0(BB_TIMER0);
BBTimer my_t1(BB_TIMER1);

bool imprimir = false;
bool leer = false;

void t0Callback()
{
	imprimir = true;
}
void t1CallBack()
{
  leer = true;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IMU.begin();
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  my_t0.setupTimer(1000000, t0Callback);
	my_t0.timerStart();
  my_t1.setupTimer(100000, t1CallBack);
  my_t1.timerStart();
}



void loop() {
  // put your main code here, to run repeatedly:
  char buffer[100];
  if (imprimir)
  {
    Serial.println("DATOS a(x,y,z);g(x,y,z);m(x,y,z)");
    for(int j = 0; j < i; j++){
    sprintf(buffer, "%f;%f;%f;%f;%f;%f;%f;%f;%f\n", x[j],y[j],z[j], xG[j],yG[j],zG[j], xM[j],yM[j],zM[j]);
    Serial.print(buffer);
    }
    imprimir =  false;
    i=0;
  }
  if (leer)
  {
    if (IMU.accelerationAvailable())
    {
      IMU.readAcceleration(x[i],y[i],z[i]);
    }
    if (IMU.gyroscopeAvailable())
    {
      IMU.readGyroscope(xG[i],yG[i],zG[i]);
    }
    if (IMU.magneticFieldAvailable())
    {
      IMU.readMagneticField(xM[i],yM[i],zM[i]);
    }
    i++;
    leer=false;
  }
}