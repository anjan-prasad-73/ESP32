const int potpin=34;
void setup() {
Serial.begin(115200);
analogReadResolution(12);

}

void loop() {
  int rew_data=analogRead(potpin);
  float voltage=raw_data *(3.3/4095.0);
  Serial.printf("raw data:%d,Voltage:%.2f",raw_data,voltage);
  delay(500);

}
