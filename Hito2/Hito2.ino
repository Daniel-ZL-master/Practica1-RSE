#include "BBTimer.hpp"
// Construct five timers.  Can construct a global scope, or within setup(), or static in loop().
BBTimer my_t0(BB_TIMER0);

// global logicals for example synchronization between timer callback functions and loop().
bool imprimir = false;
int raw_adc;

// One callback for each timer.  
void t0Callback()
{
	imprimir = true;
}



void setup() {

	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	my_t0.setupTimer(10000000, t0Callback);
	my_t0.timerStart();

}

void loop() {
	if(imprimir){
		raw_adc = analogRead(A0);
		Serial.println(raw_adc);
		imprimir=false;
	}
}