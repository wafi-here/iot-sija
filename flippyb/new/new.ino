#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set address to 0x27 for Tinkercad PCF8574
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Hardware Pins
const int BUTTON_PIN = 2;
const int BUZZER_PIN = 12;

// Custom Bitmaps
byte bird[8] = {
  B00000,
  B00100,
  B01011,
  B11110,
  B11100,
  B01111,
  B00000,
  B00000
};

byte wallTop[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000
};

byte wallBottom[8] = {
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// World Track Grid (Columns 0 to 11 for active field, 12-15 for Score HUD)
// 0 = Empty space, 1 = Top obstacle, 2 = Bottom obstacle
int track[12];

// Game Variables
int birdRow = 1;              // 0 = Top, 1 = Bottom
int score = 0;
bool gameOver = false;

// Button Tracking (Edge Triggered)
bool lastButtonReading = HIGH;
bool jumpRequested = false;

// Dynamic Speed Control (Chrome Dino Mechanics)
unsigned long lastGameUpdate = 0;
const int START_SPEED = 220;  // Initial frame delay (ms)
const int MIN_SPEED = 220;     // Max speed cap (ms)
int currentSpeed = START_SPEED;

// Obstacle Spacing Control
int minGap = 4;               // Minimum columns between obstacles (decreases with score)
int colsSinceLastWall = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, bird);
  lcd.createChar(1, wallTop);
  lcd.createChar(2, wallBottom);

  randomSeed(analogRead(0));
  showStartScreen();
}

void loop() {
  if (gameOver) {
    if (checkButtonPress()) {
      resetGame();
    }
    return;
  }

  // Poll for button input constantly
  if (checkButtonPress()) {
    jumpRequested = true;
  }

  // Frame Tick Update
  if (millis() - lastGameUpdate > currentSpeed) {
    lastGameUpdate = millis();

    // Process Jump vs Fall
    if (jumpRequested) {
      birdRow = 0;
      tone(BUZZER_PIN, 1200, 20);
      jumpRequested = false;
    } else {
      birdRow = 1; // Gravity pulls bird to bottom row
    }

    updateGameLogic();
  }
}

bool checkButtonPress() {
  bool currentReading = digitalRead(BUTTON_PIN);
  bool pressed = false;

  if (currentReading == LOW && lastButtonReading == HIGH) {
    pressed = true;
  }
  
  lastButtonReading = currentReading;
  return pressed;
}

void updateGameLogic() {
  // 1. Shift track left (scroll world)
  for (int i = 0; i < 11; i++) {
    track[i] = track[i + 1];
  }

  // 2. Increment obstacle distance counter
  colsSinceLastWall++;

  // 3. Dynamic Obstacle Spawner at right boundary (col 11)
  if (colsSinceLastWall >= minGap) {
    // 60% chance to spawn obstacle once minimum gap criteria is met
    if (random(0, 100) < 60) {
      track[11] = random(1, 3); // 1 = Top block, 2 = Bottom block
      colsSinceLastWall = 0;
    } else {
      track[11] = 0;
    }
  } else {
    track[11] = 0;
  }

  // 4. Scoring: Award point when an obstacle passes fixed bird column (col 1)
  if (track[1] > 0) {
    score++;
    tone(BUZZER_PIN, 1800, 25);

    // Speed up smoothly per score (Chrome Dino style)
    currentSpeed = max(MIN_SPEED, START_SPEED - (score * 6));

    // Tighten obstacle spacing as difficulty increases
    if (score > 10) minGap = 3;
    if (score > 25) minGap = 2;
  }

  // 5. Collision Detection (Bird is locked at Column 1)
  if ((track[1] == 1 && birdRow == 0) || (track[1] == 2 && birdRow == 1)) {
    triggerGameOver();
    return;
  }

  // 6. Draw Frame
  drawFrame();
}

void drawFrame() {
  lcd.clear();

  // Draw HUD (Columns 12-15)
  lcd.setCursor(12, 0);
  lcd.print("S:");
  if (score < 10) lcd.print("0");
  lcd.print(score);

  // Draw Scrolling Obstacles (Columns 0-11)
  for (int col = 0; col < 12; col++) {
    if (track[col] == 1) {
      lcd.setCursor(col, 0);
      lcd.write(byte(1));
    } else if (track[col] == 2) {
      lcd.setCursor(col, 1);
      lcd.write(byte(2));
    }
  }

  // Draw Bird at fixed position Column 1
  lcd.setCursor(1, birdRow);
  lcd.write(byte(0));
}

void triggerGameOver() {
  gameOver = true;
  
  tone(BUZZER_PIN, 350, 150);
  delay(150);
  tone(BUZZER_PIN, 180, 300);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER!");
  lcd.setCursor(1, 1);
  lcd.print("FINAL SCORE: ");
  lcd.print(score);
}

void resetGame() {
  score = 0;
  birdRow = 1;
  currentSpeed = START_SPEED;
  minGap = 4;
  colsSinceLastWall = 0;
  gameOver = false;

  // Clear track array
  for (int i = 0; i < 12; i++) {
    track[i] = 0;
  }

  lcd.clear();
}

void showStartScreen() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("FLAPPY BIRD");
  lcd.setCursor(1, 1);
  lcd.print("Press to Jump");

  while (digitalRead(BUTTON_PIN) == HIGH) {
    // Wait for start
  }
  
  tone(BUZZER_PIN, 1000, 80);
  
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("XYZ");
  delay(600);
  lcd.clear();
}
