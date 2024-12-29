#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<Servo.h>



// Define the motor pins
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

Servo seedservo, rotaryservo;


// Variables for the Blynk widget values
int x =0;
int y =0;
int Speed = 105;
int pumppn = D8;

char auth[] = "K5kcghuY9mTboPgFF73N7GWcqCkenx9r"; //Enter your Blynk auth token
char ssid[] = "pradeep"; //Enter your WIFI name
char pass[] = "12345678"; //Enter your WIFI passowrd


void setup() {
  Serial.begin(9600);
  //Set the motor pins as output pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
 
  pinMode(pumppn, OUTPUT);
  seedservo.attach(D7);
  rotaryservo.attach(D6);
  digitalWrite(pumppn, LOW);

  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V3) {
 rotaryservo.write(param.asInt())  ;
}

BLYNK_WRITE(V2) {
  seedservo.write(param.asInt())  ;
}
BLYNK_WRITE(V4) {
  digitalWrite(pumppn, param.asInt())  ;
}


// Check these values using the IF condition
void smartcar() {
  if (y == 1) {
    carForward();
    Serial.println("carForward");
  } else if (y == 2) {
    carBackward();
    Serial.println("carBackward");
  } else if (x == 2) {
    carLeft();
    Serial.println("carLeft");
  } else if (x == 1) {
    carRight();
    Serial.println("carRight");
  } else if (x == 0 && y == 0 ) {
    carStop();
    Serial.println("carstop");
  }
}
void loop() {
  Blynk.run();// Run the blynk function
  smartcar();// Call the main function
}

/*Motor movement functions*/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}