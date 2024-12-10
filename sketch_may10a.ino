#include <SoftwareSerial.h>
#include <TinyGPS++.h>

int buttonpin = 6;

float lattitude, longitude;

float a[2];

float *p;

SoftwareSerial gpsSerial(2, 3);

SoftwareSerial gsmSerial(10, 11);

TinyGPSPlus gps;

const int trigPin1 = A1;
const int echoPin1 = A0;
long duration1;
int distance1;

const int trigPin2 = A2;
const int echoPin2 = A3;
long duration2;
int distance2;

const int trigPin3 = A4;
const int echoPin3 = A5;
long duration3;
int distance3;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(buttonpin, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(A6, INPUT_PULLUP); // assuming this pin is for another button
  delay(1000);
  gpsSerial.begin(9600);
  delay(1000);
  gsmSerial.begin(9600);
  delay(1000);
  Serial.println("—Tracking–");
  Serial.println("***Location***");
  gsmSerial.println("AT+CNMI=2,2,0,0,0");
  delay(3000);
  Serial.println("Initializing……");
  delay(2000);
  Serial.println("System Ready  ");
  delay(1000);
}

void loop() {
  if (digitalRead(buttonpin) == LOW) {
    Serial.println("button pressed");
    delay(2000);
    SendMessage();
  }
  if (digitalRead(A6) == LOW) { // Check another button
    Serial.println("Another button pressed");
    delay(2000);
    // Do something else
  }
  if (gsmSerial.available() > 0)
    Serial.write(gsmSerial.read());
  while (gsmSerial.available())
    gsmSerial.read();
  while (Serial.available())
    Serial.read();
  get_gsm();
}

float *get_gps() {
  gpsSerial.listen();
  while (1) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    if (gps.location.isUpdated()) {
      Serial.print("LAT=");
      Serial.println(gps.location.lat(), 6);
      Serial.print("LONG=");
      Serial.println(gps.location.lng(), 6);
      lattitude = gps.location.lat();
      longitude = gps.location.lng();
      break;
    }
  }
  a[0] = lattitude;
  a[1] = longitude;
  return a;
}

void get_gsm() {
  gsmSerial.listen();
  while (gsmSerial.available() > 0) {
    if (gsmSerial.find("Track")) {
      gsmSerial.println("AT+CMGF=1");  
      delay(1000);  
      gsmSerial.println("AT+CMGS=\"+917499076198\"\r");  
      delay(1000);
      p = get_gps();
      gsmSerial.listen();
      Serial.print("Your Car Location: ");
      gsmSerial.print("Your Car Location: ");
      Serial.print("LATTITUDE=");
      Serial.print(*p, 6);
      gsmSerial.print("LATTITUDE=");
      gsmSerial.print(*p, 6);
      gsmSerial.print(",");  
      Serial.print("LONGITUDE=");
      Serial.print(*(p + 1), 6);
      gsmSerial.print("LONGITUDE=");
      gsmSerial.print(*(p + 1), 6);  
      delay(100);
      gsmSerial.println((char)26);  
      delay(1000);
    }
  }
}

void SendMessage() {
  gsmSerial.println("AT+CMGF=1");  
  delay(1000);  
  gsmSerial.println("AT+CMGS=\"+917820968563\"\r");  
  delay(1000);
  gsmSerial.println("I Am In Problem Plz Help Me");  
  delay(1000);
  p = get_gps();
  gsmSerial.listen();
  Serial.print("Your Position is : ");
  gsmSerial.print("position is : ");
  Serial.print("LATTITUDE=");
  Serial.print(*p, 6);
  gsmSerial.print("LATTITUDE=");
  gsmSerial.print(*p, 6);
  gsmSerial.print(",");  
  Serial.print("LONGITUDE=");
  Serial.print(*(p + 1), 6);
  gsmSerial.print("LONGITUDE=");
  gsmSerial.print(*(p + 1), 6);  
  delay(100);
  gsmSerial.println((char)26);  
}
