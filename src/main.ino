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

// ── Physics ───────────────────────────────────────
#define ACCEL_SCALE  0.0002
#define FRICTION     0.80
#define MAX_VEL      0.8

float ballX = 1.0;
float ballY = 1.0;
float velX  = 0.0;
float velY  = 0.0;

// ── Previous ball position ────────────────────────
int prevX = 1;
int prevY = 1;

// ── Goal ──────────────────────────────────────────
#define GOAL_X 30
#define GOAL_Y  6

// ── Maze ──────────────────────────────────────────
const bool maze[ROWS][COLS] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,1,0,1},
  {1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
  {1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1},
  {1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// ── Blink ─────────────────────────────────────────
bool goalVisible = true;
int  blinkCounter = 0;
#define BLINK_RATE 6

// ── Draw a pixel ──────────────────────────────────
void drawPixel(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) return;
  int device = x / 8;
  int colBit = 7 - (x % 8);
  matrix.setRow(device, y, matrix.getRow(device, y) | (1 << colBit));
}

// ── Clear a pixel (only if not a wall) ────────────
void clearPixel(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) return;
  if (maze[y][x]) return;  // never clear a wall
  int device = x / 8;
  int colBit = 7 - (x % 8);
  matrix.setRow(device, y, matrix.getRow(device, y) & ~(1 << colBit));
}

// ── Draw maze once ────────────────────────────────
void drawMaze() {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (maze[row][col]) drawPixel(col, row);
    }
  }
}

// ── Win animation ─────────────────────────────────
void winAnimation() {
  // Rapid flashes at goal
  for (int i = 0; i < 10; i++) {
    drawPixel(GOAL_X, GOAL_Y);
    delay(80);
    clearPixel(GOAL_X, GOAL_Y);
    delay(80);
  }
  // Full display flash x2
  for (int i = 0; i < 2; i++) {
    for (int d = 0; d < NUM_DEVICES; d++)
      for (int r = 0; r < ROWS; r++)
        matrix.setRow(d, r, 0xFF);
    delay(200);
    matrix.clear();
    delay(200);
  }
  // Reset
  ballX = 1.0; ballY = 1.0;
  velX  = 0.0; velY  = 0.0;
  prevX = 1;   prevY = 1;
  // Redraw maze after full clear
  drawMaze();
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
  drawMaze();  // draw maze once at start, never clear it again
}

void loop() {
  int16_t ax, ay, az;
  imu.getAcceleration(&ax, &ay, &az);

  // ── Physics ───────────────────────────────────
  velX += ax * ACCEL_SCALE;
  velY -= ay * ACCEL_SCALE;
  velX *= FRICTION;
  velY *= FRICTION;
  velX = constrain(velX, -MAX_VEL, MAX_VEL);
  velY = constrain(velY, -MAX_VEL, MAX_VEL);

  float nextX = ballX + velX;
  float nextY = ballY + velY;

  // ── Wall collision ────────────────────────────
  if (nextX >= 0 && nextX < COLS &&
      maze[(int)ballY][(int)nextX]) {
    velX = -velX * 0.4;
    nextX = ballX;
  }
  if (nextY >= 0 && nextY < ROWS &&
      maze[(int)nextY][(int)ballX]) {
    velY = -velY * 0.4;
    nextY = ballY;
  }

  ballX = constrain(nextX, 0, COLS - 1);
  ballY = constrain(nextY, 0, ROWS - 1);

  int drawX = (int)ballX;
  int drawY = (int)ballY;

  // ── Win check ────────────────────────────────
  if (drawX == GOAL_X && drawY == GOAL_Y) {
    winAnimation();
    return;
  }

  // ── Blink goal ───────────────────────────────
  blinkCounter++;
  if (blinkCounter >= BLINK_RATE) {
    goalVisible = !goalVisible;
    blinkCounter = 0;
  }

  // ── Draw — no matrix.clear() ─────────────────
  // Only erase previous ball position
  clearPixel(prevX, prevY);

  // Draw goal blink
  if (goalVisible) drawPixel(GOAL_X, GOAL_Y);
  else clearPixel(GOAL_X, GOAL_Y);

  // Draw ball
  drawPixel(drawX, drawY);

  // Save position for next frame
  prevX = drawX;
  prevY = drawY;

  Serial.print("X: "); Serial.print(drawX);
  Serial.print(" Y: "); Serial.println(drawY);

  delay(50);
}