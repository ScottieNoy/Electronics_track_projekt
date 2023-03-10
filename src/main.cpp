#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
TFT_eSPI tft = TFT_eSPI();
// define the pins for the HC-SR04 sensor
const int trigPin = 13; // GPIO14 on ESP32 board
const int echoPin = 36; // GPIO15 on ESP32 board

// define variables for timing the HC-SR04 sensor
long duration;
int distance;


void printToDisplay(sensors_event_t g, int distance);


void setup(void) {
  Serial.begin(115200);
  // set the trigger pin as an output
  pinMode(trigPin, OUTPUT);
  
  // set the echo pin as an input
  pinMode(echoPin, INPUT);
  tft.begin();
  tft.setRotation(3);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    // Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  // Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  

  Serial.println("");
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(";");
  // Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(";");

  // Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.print(";");

  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  Serial.print(g.orientation.roll);
  Serial.print(";");

  // Serial.print(", Y: ");
  Serial.print(g.orientation.pitch);
  Serial.print(";");
  // Serial.print(", Z: ");
  Serial.print(g.orientation.heading);
  Serial.print(";");
  // Serial.println(" degrees");

  // Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.print(";");
  // Serial.println(" degC");

  // Serial.println("");

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure the duration of the pulse received on the echo pin
  duration = pulseIn(echoPin, HIGH);

  // calculate the distance in centimeters based on the duration
  distance = duration * 0.034 / 2;

  // print the distance to the serial monitor
  // Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(";");
  // Serial.println(" cm");
  printToDisplay(g,distance);
  delay(500);
}

void printToDisplay(sensors_event_t g,int distance) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1.5);
  tft.println("Rotation");
  tft.setTextColor(TFT_BLUE);
  tft.print("X:");
  tft.println(g.orientation.roll);
  // tft.println(" rad/s");
  tft.setTextColor(TFT_RED);
  tft.print("Y:");
  tft.println(g.orientation.pitch);
  // tft.println(" rad/s");
  tft.setTextColor(TFT_GREEN);
  tft.print("Z:");
  tft.println(g.orientation.heading);
  // tft.println(" rad/s");
  tft.setTextColor(TFT_YELLOW);
  tft.print("Dist:");
  tft.print(distance);
  tft.println(" cm");

}