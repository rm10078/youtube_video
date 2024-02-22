#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
const int irSensorPin = 2; // IR sensor input pin (D2)
const int interruptPin = 0; // Interrupt pin for IR sensor (D2 on Nano)
const int screenResetPin = -1; // Reset pin for OLED display
const int screenAddress = 0x3C; // I2C address of OLED display
const int button=5; //the button is connected with D5 pin
const int led=13; //indicator led in D13 pin

volatile unsigned long lastTime = 0;
volatile unsigned long totalTime = 0;
volatile float rpm = 0,rpm_value=0;
int button_state=0;

Adafruit_SSD1306 display(128, 64, &Wire,screenResetPin);

void handleInterrupt() {
  unsigned long currentTime = micros();
  totalTime = currentTime - lastTime;
  lastTime = currentTime;
}

void setup() {
  pinMode(led,OUTPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(button,INPUT_PULLUP);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(irSensorPin), handleInterrupt, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, screenAddress);
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("RPM Meter");
  display.display();
}

void loop() {
  if (totalTime > 0) {
    rpm = 60000000.0 /totalTime;
  }
  if(digitalRead(button)==0){ //Logic for button action (When the button is press.)
    delay(500);
    while(button==0){}
    if(button_state==0){
      button_state=1;
    }
    else{
      button_state=0;
    }
  }

  if(button_state==0){
  rpm_value=rpm;
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 0);
  display.print("RPM: ");
  if(button_state){
  display.println("L");
  digitalWrite(led,HIGH);
  }
  else{
  display.println("UL");
  digitalWrite(led,LOW);
  }
  display.println(rpm_value, 2);
  display.display();
  delay(500);
}
