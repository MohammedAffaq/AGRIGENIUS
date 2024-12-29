#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<HCSR04.h>
#include <DHT.h>
#include<Servo.h>

       //pins Connections
#define DHTPIN 2    // DHT11 sensor pin
  // Ultrasonic sensor trig pin
#define trig D6
#define echo D5
 // Ultrasonic sensor echo pin
#define soil A0  // Soil moisture sensor pin
#define RELAY_PIN_1  D3   // Relay control pin
#define relayVirtualPin V4  // Virtual pin to control the 
#define VPIN_BUTTON_1    V4 

HCSR04 hc(trig,echo);
double T, P;
char status;
int relay1State = LOW;

#define BLYNK_TEMPLATE_ID "TMPL378rjHtZo"
#define BLYNK_TEMPLATE_NAME "Agri monitor"
#define BLYNK_AUTH_TOKEN "iy6jqj3f8faoL2I7ly0hN8gQrYG0y_yK"
 

char auth[] = BLYNK_AUTH_TOKEN;  // Blynk authentication token
char ssid[] = "pradeep";   // WiFi SSID
char pass[] = "12345678";  // WiFi password


DHT dht(D2, DHT11);//(DHT sensor pin,sensor type)  D4 DHT11 Temperature Sensor
BlynkTimer timer;

Servo soilservo, cutterservo;

void setup()

{
  // Debug console
  Serial.begin(9600);
 Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
Serial.println("start");
soilservo.attach(D1);
  cutterservo.attach(D7);
 pinMode(RELAY_PIN_1, OUTPUT);
 digitalWrite(RELAY_PIN_1, LOW);
 
  //Call the function
timer.setInterval(1000L, sendSensor);
timer.setInterval(100L, soilMoistureSensor);
timer.setInterval(100L, DHT11sensor);
}





BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

BLYNK_WRITE(V5) {
  soilservo.write(param.asInt())  ;
}
BLYNK_WRITE(V6) {
  cutterservo.write(param.asInt())  ;
}

void loop()

{

  Blynk.run();

  timer.run();    }



//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}



//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  Blynk.virtualWrite(V3, value);
}




void sendSensor()
{
  int c = hc.dist();

  if ( c == 0 ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 Serial.println(c);
  Blynk.virtualWrite(V2, c);  // select virtual pin (v5) in blynk app
}