const int button=4;
void setup() {
  Serial.brign(115200);
  pinMode(button,INPUT);

}

void loop() {
  bool read=digitalRead(button);
  if(read)
  {
    Serial.println("Button pressed");
  }
  else
  {
    Serial.println("Not Pressed");
  }
}
