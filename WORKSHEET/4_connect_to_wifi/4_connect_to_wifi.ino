#include <WiFi.h>
const char* ssid ="Redmi_10";
const char *password="123456789";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to WiFi");
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("CONNECTED TO IP:");
  Serial.print(WiFi.localIP());

}

void loop() {
 if(WiFi.status() != WL_CONNECTED)
{
  Serial.println("WiFi disconnected ,reattempting...");
  WiFi.reconnect();
}
delay(2000);
}
