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

// ── Physics constants — tweak these to change feel ─
#define ACCEL_SCALE  0.0003  // how strongly tilt pushes ball
#define FRICTION     0.97    // 1.0 = no friction, 0.0 = instant stop
#define MAX_VEL      1.5     // speed cap

// ── Ball state ─────────────────────────────────────
float ballX = 16.0;
float ballY =  4.0;
float velX  =  0.0;  // NEW — ball now has velocity
float velY  =  0.0;

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

  // 1. Tilt adds force to velocity
  //    Before: tilt SET position directly
  //    Now:    tilt PUSHES the ball, velocity builds up
  velX += ax * ACCEL_SCALE;
  velY -= ay * ACCEL_SCALE;

  // 2. Friction — shrinks velocity each frame
  //    Without this ball slides forever
  velX *= FRICTION;
  velY *= FRICTION;

  // 3. Speed cap — prevents ball flying too fast
  velX = constrain(velX, -MAX_VEL, MAX_VEL);
  velY = constrain(velY, -MAX_VEL, MAX_VEL);

  // 4. Move ball by its velocity
  ballX += velX;
  ballY += velY;

  // 5. Bounce off walls
  //    Flip velocity and lose some energy on impact
  if (ballX < 0)        { ballX = 0;        velX = -velX * 0.6; }
  if (ballX > COLS - 1) { ballX = COLS - 1; velX = -velX * 0.6; }
  if (ballY < 0)        { ballY = 0;        velY = -velY * 0.6; }
  if (ballY > ROWS - 1) { ballY = ROWS - 1; velY = -velY * 0.6; }

  // 6. Draw
  int drawX = constrain((int)ballX, 0, COLS - 1);
  int drawY = constrain((int)ballY, 0, ROWS - 1);
  drawPixel(drawX, drawY);

  Serial.print("velX: "); Serial.print(velX);
  Serial.print(" velY: "); Serial.print(velY);
  Serial.print(" -> X: "); Serial.print(drawX);
  Serial.print(" Y: "); Serial.println(drawY);

  delay(50);
}