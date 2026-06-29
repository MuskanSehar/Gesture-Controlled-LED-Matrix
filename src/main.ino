#include <Wire.h>
#include <MPU6050.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define DATA_PIN    23
#define CS_PIN       5
#define CLK_PIN     18
#define NUM_DEVICES  4

MPU6050 imu;
MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::FC16_HW,
                                DATA_PIN, CLK_PIN, CS_PIN,
                                NUM_DEVICES);

#define COLS 32
#define ROWS  8
#define LERP 0.1

float smoothX = 16.0;
float smoothY =  4.0;

void drawPixel(int x, int y) {
  matrix.clear();
  int device = x / 8;
  int colBit = 7 - (x % 8);
  uint8_t rowByte = (1 << colBit);
  matrix.setRow(device, y, rowByte);
}

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

  // Map full int16 range to grid
  int targetX;
  if (ax >= 32767)       targetX = 31;
  else if (ax <= -32768) targetX = 0;
  else                   targetX = constrain(map(ax, -32768, 32767, 0, 32), 0, 31);

  int targetY = constrain(map(ay, -32768, 32767, ROWS - 1, 0), 0, ROWS - 1);

  // Snap to edges, lerp everywhere else
  if (targetX == 31)       smoothX = 31;
  else if (targetX == 0)   smoothX = 0;
  else                     smoothX += (targetX - smoothX) * LERP;

  if (targetY == ROWS - 1) smoothY = ROWS - 1;
  else if (targetY == 0)   smoothY = 0;
  else                     smoothY += (targetY - smoothY) * LERP;

  int drawX = constrain((int)smoothX, 0, COLS - 1);
  int drawY = constrain((int)smoothY, 0, ROWS - 1);

  drawPixel(drawX, drawY);

  Serial.print("AX: "); Serial.print(ax);
  Serial.print(" AY: "); Serial.print(ay);
  Serial.print(" -> X: "); Serial.print(drawX);
  Serial.print(" Y: "); Serial.println(drawY);

  delay(50);
}