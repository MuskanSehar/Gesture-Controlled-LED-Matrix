#include <Wire.h>
#include <MPU6050.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// MAX7219 Pins
#define DATA_PIN 23
#define CS_PIN   5
#define CLK_PIN  18
#define NUM_DEVICES 1

// Create objects
MPU6050 imu;
MD_MAX72XX matrix =
  MD_MAX72XX(MD_MAX72XX::FC16_HW,
             DATA_PIN,
             CLK_PIN,
             CS_PIN,
             NUM_DEVICES);

int dotX = 3;
int dotY = 3;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  imu.initialize();
  if (!imu.testConnection()) {
    Serial.println("MPU6050 not connected!");
  } else {
    Serial.println("MPU6050 connected!");
  }
  matrix.begin();
  matrix.clear();
}

void loop() {
  int16_t ax, ay, az;
  imu.getAcceleration(&ax, &ay, &az);

  dotX = constrain(map(ax, -17000, 17000, 0, 7), 0, 7);
  dotY = constrain(map(ay, -17000, 17000, 7, 0), 0, 7);

  matrix.clear();
  matrix.setPoint(dotY, dotX, true);

  Serial.print("AX: ");
  Serial.print(ax);
  Serial.print(" AY: ");
  Serial.print(ay);
  Serial.print(" -> X: ");
  Serial.print(dotX);
  Serial.print(" Y: ");
  Serial.println(dotY);

  delay(50);
}
