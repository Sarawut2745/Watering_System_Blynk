//เพิ่มไลบรารี
#define Pump D0
#define SW_PIN D3
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

////กำหนดตัวเเปร
char auth[] = "oEazKTnhugwJMc-SbnZu56ssPqAQYZJt";
bool Check = false;

WidgetLCD lcd1(V2);
WidgetLED LED_RED(V3);
WidgetLED LED_BLUE(V4);

////ฟังชั่น เปิดปั้มน้ำ ระบบมือ
BLYNK_WRITE(V1) {

  int pinValue = param.asInt();
  Serial.print("Value is: ");
  Serial.println(param.asInt());

  if (pinValue == 1) {
    lcd1.print(3, 1, "Pump On");
    LED_RED.on();
    LED_BLUE.off();
    digitalWrite(Pump, HIGH);
    Check = true;
  } if (pinValue == 0) {
    lcd1.print(3, 1, "Pump Off");
    LED_RED.off();
    LED_BLUE.on();
    digitalWrite(Pump, LOW);
    Check = false;
  }
}
void setup()
{
  Serial.begin(9600);
  pinMode(Pump, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  WiFiManager wifiManager;
  
  Serial.println("Reset wifi config?:");
  for (int i = 5; i > 0; i--) {
    Serial.print(String(i) + " ");
    delay(1000);
  }

  if (digitalRead(SW_PIN) == LOW) // Press button
  {
    Serial.println();
    
    Serial.println("Reset wifi config");
    
    wifiManager.resetSettings();
  }

  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
  
  wifiManager.autoConnect("Smart Farm");
  
  Serial.println("connected...yeey :)");
  
  Blynk.config(auth);
}


void loop() {

  Blynk.run();

  float temp = analogRead(A0);

  Blynk.virtualWrite(V0, temp);

  delay(1000);

  ////ระบบอัตโนมัติรดน้ำต้นไม้
  if (temp >= 700 && Check == true) {
    lcd1.print(3, 1, "Pump On");
    LED_RED.on();
    LED_BLUE.off();
    digitalWrite(Pump, HIGH);
    Check = false;

  }

  if (temp <= 700 && Check == false) {
    lcd1.print(3, 1, "Pump Off");
    LED_RED.off();
    LED_BLUE.on();
    digitalWrite(Pump, LOW);
    Check = true;

  }

}