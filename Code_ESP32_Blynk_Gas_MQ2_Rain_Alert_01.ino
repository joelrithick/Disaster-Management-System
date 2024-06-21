/**********************************************************************************
 *  TITLE: MQ2 Gas Leakage & Rain Detector with Blynk IoT using ESP32
 *  Click on the following links to learn more. 
 *  YouTube Video: 
 *  Related Blog : https://iotcircuithub.com/mq2-gas-sensor-rain-detection-blynk-notification
 *  by Tech StudyCell
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP32 : https://github.com/espressif/arduino-esp32
 *
 *  Download the libraries 
 *  Blynk 1.0.1 Library:  https://github.com/blynkkk/blynk-library
 **********************************************************************************/

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "OnePlus 9R";
char pass[] = "q9dfii6m";

// define the GPIO connected with Sensors & LEDs
#define MQ2_SENSOR    35
#define RAIN_SENSOR   34
#define GREEN_LED     14
#define RED_LED       25
#define WIFI_LED      2

//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

int MQ2_SENSOR_Value = 0;
int RAIN_SENSOR_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, HIGH);
    sendData();
    //Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, LOW);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 4095, 0, 100);
  RAIN_SENSOR_Value = digitalRead(RAIN_SENSOR);
  
  if (MQ2_SENSOR_Value > 50 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else if (RAIN_SENSOR_Value == 0 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else{
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}

void sendData()
{  
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  if (MQ2_SENSOR_Value > 50 )
  {
    Blynk.logEvent("gas", "Gas Detected!");
  }
  else if (RAIN_SENSOR_Value == 0 )
  {
    Blynk.logEvent("rain", "Water Detected!");
    Blynk.virtualWrite(VPIN_BUTTON_2, "Water Detected!");
  }
  else if (RAIN_SENSOR_Value == 1 )
  {
    Blynk.virtualWrite(VPIN_BUTTON_2, "No Water Detected.");
  } 
}

void setup()
{
  Serial.begin(9600);
 
  pinMode(MQ2_SENSOR, INPUT);
  pinMode(RAIN_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(WIFI_LED, LOW);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}

void loop()
{
  getSensorData();
  Blynk.run();
  timer.run();
}
