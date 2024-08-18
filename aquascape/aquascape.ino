//////////////////////////////Sensor Suhu////////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SparkFunTSL2561.h>
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int motor1Pin1 = 26; 
int motor1Pin2 = 25; 
int enable1Pin = 27; 

#include <RBDdimmer.h>
const int zeroCrossPin = 33; // Pin for zero-cross detection 
const int acdPin = 32;       // Pin for AC dimmer control
int MIN_POWER = 0;          // Minimum power level
int MAX_POWER = 100;         // Maximum power level
//int POWER_STEP = 33;         // Step size for changing power level

// Variables
int power = 0; // Variable to store current power level

// Objects
dimmerLamp acd(acdPin, zeroCrossPin); // Create a dimmerLamp object with both pins


//////////////////////////////Sensor PH/////////////////////////////////////////////
#define pinPH 36		//the esp gpio data pin number
unsigned long millisView = 0;
int numReadings = 100;
char buff[10];
#include "fuzzyph.h"
#include "fuzzylux.h"
#include "fuzzysuhu.h"

//////////////////////////////////sensor lux/////////////////////////////////////////
#include <SparkFunTSL2561.h>
SFE_TSL2561 light;
boolean gain; //gain 0=X1, 1=X16
unsigned int ms;  //waktu integrasi
const int freq = 5000;
const int freq1 = 5000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;
//const int resolution1 = 8;
int ledPin = 14;

//////////////////////////////////Koneksi Firebase & Wifi/////////////////////////////
#if defined(ESP32)
#include <WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define WIFI_SSID "BESTNET"
#define WIFI_PASSWORD "Rukun123"
#define API_KEY "AIzaSyDZYlRaUkX6O1AXXer68HFQqX4sIbmx3Ww"
#define DATABASE_URL "https://aquascape-76184-default-rtdb.firebaseio.com/" 
//DFRobot_ESP_PH ph;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

////////////////////////////Alamat i2C LCD 20x4/////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 20,4);

///////////////////////////Relay
#define pumpUp_pin 23
#define pumpDown_pin 15
///////Fuzzy/////////////////////

#include <Fuzzy.h>
Fuzzy *fuzzy = new Fuzzy();
Fuzzy *fuzsuhu = new Fuzzy();



void setup(){
  
  Serial.begin(115200);
  lcd.init();
  Wire.begin();
  lcd.backlight(); //LCD
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  ///////////////////////sensor ph
  //EEPROM.begin(32);
	//ph.begin();
  light.begin(0x49);
  light.begin();
  pinMode(pinPH, INPUT);
  analogReadResolution(10);
  pinMode(pumpUp_pin, OUTPUT);
  pinMode(pumpDown_pin, OUTPUT);
  digitalWrite(pumpUp_pin, HIGH);
  digitalWrite(pumpDown_pin, HIGH);
  //////////////////////sensorlux
  unsigned char ID;
  gain = 0;
  unsigned char time = 2;
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);
  Serial.println("Powerup...");
  light.setPowerUp();
  pinMode(ledPin, OUTPUT); ///////////led hpl
  ////////////////////////////////////deklarasi pin L298N
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  ledcSetup(pwmChannel1, freq1, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  ledcAttachPin(ledPin, pwmChannel1);
  ledcAttachPin(enable1Pin, pwmChannel2);

  acd.begin(NORMAL_MODE, ON); // NORMAL_MODE for standard dimming mode, ON to start dimmer
 ////////////////////fuzzy suhu
  //fuzzysuhu(); 
  sensors.begin();
  sensors.setResolution(10);
  setfirebase();
  
}
  
void loop(){
  sensorph();
  sensorlux();
  sensorsuhu();
  delay(60000);
}
