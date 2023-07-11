#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define TRIGGER_PIN 2   // Arduino pin connected to the ultrasonic sensor's trigger pin
#define ECHO_PIN 3      // Arduino pin connected to the ultrasonic sensor's echo pin
#define button 4        //Value reset button

#define OLED_RESET -1   // Reset pin for the OLED display (not used)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
float total_distance=0;

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(button,INPUT_PULLUP); //I set the button pinPullup for ignor the 10k resistor 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize the OLED display
  display.clearDisplay(); // Clear the display buffer
  display.setTextColor(WHITE); // Set text color to white
  display.setTextSize(2); // Set text size to 1
  display.setCursor(0, 0); // Set cursor position to top-left

  while(digitalRead(button)==1){
    long duration;
  
  digitalWrite(TRIGGER_PIN, LOW); // Ensure the trigger pin is low
  delayMicroseconds(2);
  
  digitalWrite(TRIGGER_PIN, HIGH); // Send a 10 microsecond pulse to trigger the ultrasonic sensor
  delayMicroseconds(10);
  
  digitalWrite(TRIGGER_PIN, LOW); // Turn off the trigger pin
  
  duration = pulseIn(ECHO_PIN, HIGH); // Measure the duration of the echo pulse
  total_distance = duration / 58; // Calculate the distance in centimeters

  display.clearDisplay(); // Clear the previous display
  display.setCursor(0, 0);
  display.print("Distance: ");
  display.print(total_distance);
  display.print(" cm");
  display.display(); // Update the display with new content
  
  delay(500); // Wait for a moment before taking the next measurement
  }
}

void loop() {
  long duration, distance_cm;
  float height_cm=0,height_f=0;
  digitalWrite(TRIGGER_PIN, LOW); // Ensure the trigger pin is low
  delayMicroseconds(2);
  
  digitalWrite(TRIGGER_PIN, HIGH); // Send a 10 microsecond pulse to trigger the ultrasonic sensor
  delayMicroseconds(10);
  
  digitalWrite(TRIGGER_PIN, LOW); // Turn off the trigger pin
  
  duration = pulseIn(ECHO_PIN, HIGH); // Measure the duration of the echo pulse
  distance_cm = duration / 58; // Calculate the distance in centimeters
  height_cm=total_distance-distance_cm;
  height_f=height_cm*0.0328084; // Calculate the distance in foot
  display.clearDisplay(); // Clear the previous display
  display.setCursor(0, 0);
  display.print("Height: ");
  display.print(height_cm);
  display.print(" cm");
  display.setCursor(0, 50);
  display.print(height_f);
  display.print(" Foot");
  display.display(); // Update the display with new content
  
  delay(500); // Wait for a moment before taking the next measurement
}
