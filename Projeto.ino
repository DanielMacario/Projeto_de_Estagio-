#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

#define FIREBASE_HOST "testedaniuni-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "1YvvAXTDNHLinYdpSEYCzqddcUG5aId4FFOwnzo6"
#define WIFI_SSID "wifi-Alternativa!"
#define WIFI_PASSWORD "A4536095"

#define DHTPIN 5
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);


float t = 0.0;
float h = 0.0;
int p = 0.0;
float pr = 0.0;
float a = 0.0;
float ar = 0.0;
float gas = 0.0;
String chuva;

int pino_d = D0; 
int pino_a = A0; 
int val_d = 0;   
int val_a = 0;   

unsigned long previousMillis = 0;
const long interval = 10000;

void setup()
{

  Serial.begin(9600);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.reconnectWiFi(true);

  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);
}

void loop()
{
  t = dht.readTemperature();
  h = dht.readHumidity();
  Serial.print("temperatura:");
  Serial.println(t);
  Serial.print("humidade");
  Serial.println(h);
  Firebase.setFloat("/app/temp", t);
  Firebase.setFloat("/app/umid", h);
  Serial.println("0");

  p = bmp.readPressure();
  Serial.print("pressao:");
  Serial.println(p);
  pr = bmp.readSealevelPressure();
  Serial.print("Pressao Relativa:");
  Serial.println(pr);
  a = bmp.readAltitude();
  Serial.print("Altitude: ");
  Serial.println(a);
  ar = bmp.readAltitude(102000);
  Serial.println("Altitude Rela:");
  Serial.println(ar);
  Firebase.setInt("/app/pres", p);
  Firebase.setFloat("/app/presR", pr);
  Firebase.setFloat("/app/alti", a);
  Firebase.setFloat("/app/altiR", ar);

  float x = analogRead(A0);
  gas = h / 1023 * 100;
  Firebase.setFloat("/app/gas", gas);

  val_d = digitalRead(pino_d);
  val_a = analogRead(pino_a);
  if ( val_a < 300) {       // Chuva intensa
    chuva = "Chuva Intensa";
    Firebase.setString("/app/chuva", chuva);
  }
  if (val_a <= 500 && val_a >= 300) { // Chuva moderada
    chuva = "Chuva Moderada";
    Firebase.setString("/app/chuva", chuva);
  }
  // Se a leitura analógica for maior que 500
  if ( val_a > 500) {        // Sem previsão de Chuva
    chuva = "Sem Chuva";
    Firebase.setString("/app/chuva", chuva);
  }
  Serial.println(chuva);
  delay(200);
}
