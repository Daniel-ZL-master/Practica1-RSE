void setup(){
    Serial.begin(9600);
}

void loop(){
    int raw_adc = analogRead(A0);
    Serial.println(raw_adc);
    delay(1000);
}
