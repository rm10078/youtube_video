#include <SoftwareSerial.h>
#include <TinyGPS.h>
#define button 4
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

float flat, flon,lat1,lon1;
unsigned long age;
bool newData = false;

TinyGPS gps;
SoftwareSerial ss(2, 3);
#define OLED_RESET -1   // Reset pin for the OLED display (not used)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const double EARTH_RADIUS_KM = 6371.0; // Earth's radius in kilometers

// Function to convert degrees to radians
double toRadians(double degrees) {
  return degrees * PI / 180.0;
}

// Function to calculate the distance using the Haversine formula
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
  double dlat = toRadians(lat2 - lat1);
  double dlon = toRadians(lon2 - lon1);

  double a = sin(dlat / 2) * sin(dlat / 2) +
             cos(toRadians(lat1)) * cos(toRadians(lat2)) *
             sin(dlon / 2) * sin(dlon / 2);

  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = EARTH_RADIUS_KM * c;

  return distance;
}


void gps_data_update(){
    for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

}}

void display_lat_lon(float lat,float lon){
  display.clearDisplay(); // Clear the display buffer
  display.setCursor(0, 0); // Set cursor position to top-left
  display.setTextSize(1);
  display.print("Lat:");
  display.print(lat);
  display.print("\n");
  display.print("Lon:");
  display.print(lon);
  display.display();
}

void display_distance(float dis){
  display.clearDisplay();
  display.setCursor(0, 0); // Set cursor position to top-left
  display.print("Distance:\n");
  display.setTextSize(2);
  display.print(dis);
  display.setTextSize(1);
  display.print(" km\n\n");
  display.print(flat);
  display.print(" ");
  display.print(flon);
  display.display();
}
void setup(){
  Serial.begin(9600);
  ss.begin(9600);
  pinMode(button,INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize the OLED display
  display.clearDisplay(); // Clear the display buffer
  display.setTextColor(WHITE); // Set text color to white
  display.setTextSize(2); // Set text size to 1
  display.setCursor(0, 0); // Set cursor position to top-left
  display.print("OverCurrent");
  display.setTextSize(1); // Set text size to 1
  display.display();
  delay(2000);
  while(1){
    gps_data_update();
    display_lat_lon(flat,flon);
    if(digitalRead(button)==0){
      lat1=flat;
      lon1=flon;
      Serial.println("button is pressed.");
      break;
    }
  }
}

void loop(){
gps_data_update();
float dis=calculateDistance(lat1,lon1,flat, flon);
Serial.print("distance:");
Serial.println(dis);
display_distance(dis);
}