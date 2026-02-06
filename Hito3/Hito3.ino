unsigned long tprev_ON;
unsigned long tprev_OFF;
int t_ON;
int t_OFF;
float duty;
int estado = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int raw_adc = analogRead(A0);
  int voltaje = raw_adc * 3300 / 1024;
  unsigned long tactual = micros();
  duty = (float)voltaje / 3300.0;
  t_ON = duty * 200;
  t_OFF = 200 - t_ON;
  switch (estado) {
    case 0:
      digitalWrite(2, HIGH);
      tprev_ON = tactual;
      estado = 1;
      break;
    case 1:
      Serial.println(1);
      if (tactual - tprev_ON >= t_ON) {
        estado = 2;
      }
      break;
    case 2:
      digitalWrite(2, LOW);
      tprev_OFF = tactual;
      estado = 3;
      break;
    case 3:
      Serial.println(0);
      if (tactual - tprev_OFF >= t_OFF) {
        estado = 0;
      }
      break;
  }
}
