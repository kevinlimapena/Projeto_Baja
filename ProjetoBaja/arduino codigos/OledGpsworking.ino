#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definições dos pinos e configurações do OLED e GPS
#define GPS_RX 4
#define GPS_TX 3
#define GPS_BAUD 9600
#define OLED_RESET -1  // Alguns displays não têm um pino de reset, use -1

// Inicialização dos objetos
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(GPS_BAUD);
  gpsSerial.begin(GPS_BAUD);
  
  // Inicializa o display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Trava o programa se o display não for encontrado
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Inicializando...");
  display.display();
}

void loop() {
  bool newData = false;

  // Parse GPS data for one second
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (gpsSerial.available()) {
      char c = gpsSerial.read();
      if (gps.encode(c)) {
        newData = true;
      }
    }
  }

  // Se novos dados foram recebidos, atualize o display
  if (newData) {
    float flat = gps.location.lat();
    float flon = gps.location.lng();
    float speed = gps.speed.kmph();
    int sats = gps.satellites.value();
    int hdop = gps.hdop.value();

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    
    display.print("Lat: ");
    display.print(flat, 6);
    display.setCursor(0, 10);
    display.print("Vel: ");
    display.print(speed/3.6, 2);
    display.print(" m/s");

    display.setCursor(0, 20);
    
    display.print("Vel: ");
    display.print(speed, 2);
    display.print(" km/h");

    display.display();

    Serial.print("Lat: ");
    Serial.print(flat, 6);
    Serial.print(" Lon: ");
    Serial.print(flon, 6);
    Serial.print(" Sat: ");
    Serial.print(sats);
    Serial.print(" Prec: ");
    Serial.print(hdop);
    Serial.print(" Speed: ");
    Serial.print(speed, 2);
    Serial.println(" km/h");
  }
}
