#define BLYNK_TEMPLATE_ID    "TMPL3zw9y5aSH"
#define BLYNK_TEMPLATE_NAME  "under ground cable fault detection"
#define BLYNK_AUTH_TOKEN     "f_pvbaIzwv6AJvASatcoem_SfU6SKaSP" 
#define BLYNK_HEARTBEAT      60 

#define WIFI_SSID  "Test437"
#define WIFI_PASS  "logout437"

#define VPIN_FAULT_LINE  V1
#define VPIN_FAULT_KM    V2
#define VPIN_TEMPERATURE V5
#define VPIN_LATITUDE    V3
#define VPIN_LONGITUDE   V4

// NEW: Virtual Pins for LEDs
#define VPIN_LED_L1      V6
#define VPIN_LED_L2      V7
#define VPIN_LED_L3      V8

#define ESP_RX  2
#define ESP_TX  3

// 9 Switches
#define SW1  4  
#define SW2  5  
#define SW3  6  
#define SW4  7  
#define SW5  8  
#define SW6  12 
#define SW7  A1 
#define SW8  A2 
#define SW9  A3 

// Outputs
#define LED_L1  10 
#define LED_L2  11 
#define LED_L3  13 
#define BUZZER  9
#define TEMP_PIN A0

#define BLYNK_SEND_TIME  5000 

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial EspSerial(ESP_RX, ESP_TX);
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);

LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

int faultLine = 0; 
int faultKm = 0;
int lastFaultLine = -1; 
int lastFaultKm = -1;
bool lastBlynkState = false;

void sendData() {
  if (Blynk.connected()) {
    float tempC = (analogRead(TEMP_PIN) * 0.488);
    Blynk.virtualWrite(VPIN_TEMPERATURE, tempC);
    
    Blynk.virtualWrite(VPIN_FAULT_LINE, faultLine);
    Blynk.virtualWrite(VPIN_FAULT_KM, faultKm);

    // Sync LEDs (V6, V7, V8)
    Blynk.virtualWrite(V6, (faultLine == 1) ? 255 : 0);
    Blynk.virtualWrite(V7, (faultLine == 2) ? 255 : 0);
    Blynk.virtualWrite(V8, (faultLine == 3) ? 255 : 0);

    // PUSH FIXED COORDS TO V3 AND V4
    if (faultLine > 0) {
      Blynk.virtualWrite(VPIN_LATITUDE, 13.6233);
      Blynk.virtualWrite(VPIN_LONGITUDE, 79.2898);
    } else {
      Blynk.virtualWrite(VPIN_LATITUDE, 0.0);
      Blynk.virtualWrite(VPIN_LONGITUDE, 0.0);
    }
  }
}

void processHardware() {
  faultLine = 0; faultKm = 0;
  
  if (!digitalRead(SW1))      { faultLine = 1; faultKm = 1; }
  else if (!digitalRead(SW2)) { faultLine = 1; faultKm = 2; }
  else if (!digitalRead(SW3)) { faultLine = 1; faultKm = 3; }
  else if (!digitalRead(SW4)) { faultLine = 2; faultKm = 1; }
  else if (!digitalRead(SW5)) { faultLine = 2; faultKm = 2; }
  else if (!digitalRead(SW6)) { faultLine = 2; faultKm = 3; }
  else if (!digitalRead(SW7)) { faultLine = 3; faultKm = 1; }
  else if (!digitalRead(SW8)) { faultLine = 3; faultKm = 2; }
  else if (!digitalRead(SW9)) { faultLine = 3; faultKm = 3; }

  // Output Matrix
  digitalWrite(LED_L1, (faultLine == 1) ? HIGH : LOW);
  digitalWrite(LED_L2, (faultLine == 2) ? HIGH : LOW);
  digitalWrite(LED_L3, (faultLine == 3) ? HIGH : LOW);
  digitalWrite(BUZZER, (faultLine > 0) ? HIGH : LOW);

  bool currentBlynkState = Blynk.connected();
  if (faultLine != lastFaultLine || faultKm != lastFaultKm || currentBlynkState != lastBlynkState) {
    lcd.clear();
    lcd.setCursor(0,0);
    
    if (faultLine == 0) {
      lcd.print("Status: Normal  ");
      lcd.setCursor(0,1);
      lcd.print(currentBlynkState ? "Blynk: Online   " : "Blynk: Offline  ");
    } else {
      lcd.print("FAULT: L"); lcd.print(faultLine); lcd.print(" "); lcd.print(faultKm); lcd.print("km ");
      lcd.setCursor(0,1);
      // REPLACED ALERT TRIGGERED WITH FIXED COORDS
      lcd.print("13.6233, 79.2898"); 
    }
    
    lastFaultLine = faultLine;
    lastFaultKm = faultKm;
    lastBlynkState = currentBlynkState;
  }
}

void setup() {
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  lcd.init();
  lcd.backlight();
  lcd.print("System Booting");

  pinMode(SW1, INPUT_PULLUP); pinMode(SW2, INPUT_PULLUP); pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP); pinMode(SW5, INPUT_PULLUP); pinMode(SW6, INPUT_PULLUP);
  pinMode(SW7, INPUT_PULLUP); pinMode(SW8, INPUT_PULLUP); pinMode(SW9, INPUT_PULLUP);
  
  pinMode(LED_L1, OUTPUT); pinMode(LED_L2, OUTPUT); pinMode(LED_L3, OUTPUT); pinMode(BUZZER, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, WIFI_SSID, WIFI_PASS, "blynk.cloud", 80);

  timer.setInterval(BLYNK_SEND_TIME, sendData); 
  timer.setInterval(200L, processHardware); 
}

void loop() {
  Blynk.run();
  timer.run();
}
