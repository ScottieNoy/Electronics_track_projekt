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

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.orientation.roll);
  Serial.print(", Y: ");
  Serial.print(g.orientation.pitch);
  Serial.print(", Z: ");
  Serial.print(g.orientation.heading);
  Serial.println(" degrees");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");

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
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  printToDisplay(g,distance);
  delay(500);
}

void printToDisplay(sensors_event_t g,int distance) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Rotation");
  tft.setTextColor(TFT_BLUE);
  tft.print("X:");
  tft.print(g.orientation.roll);
  // tft.println(" rad/s");
  tft.setTextColor(TFT_RED);
  tft.print("Y:");
  tft.print(g.orientation.pitch);
  // tft.println(" rad/s");
  tft.setTextColor(TFT_GREEN);
  tft.print("Z:");
  tft.print(g.orientation.heading);
  // tft.println(" rad/s");
  tft.setTextColor(TFT_YELLOW);
  tft.print("Dist:");
  tft.print(distance);
  tft.println(" cm");

}