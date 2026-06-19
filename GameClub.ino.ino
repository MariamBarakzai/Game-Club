                                                               #include <Arduino.h>
#include <U8x8lib.h>
#include "LedControl.h"

// Hardware Pins für Grove Beginner Kit
LedControl lc = LedControl(2, 4, 3, 1); 
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

const int buttonPin = 6;
const int buzzerPin = 5;
const int ledPin = 4; // Die rote LED auf dem Grove Board

int gameMode = 1; 
unsigned long lastMoveTime = 0;
unsigned long buttonTimer = 0; 
bool cheatTriggered = false;

int currentLevel, blockWidth, blockPos, direction;
int stackWidth[8], stackPos[8], moveDelay;
int playerPos, itemRow, itemCol, score;
int wallRow, gapPos, dodgeSpeed;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT); // LED als Ausgang setzen
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  
  u8x8.begin();
  u8x8.setFlipMode(1);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  resetVariables(); 
}

void loop() {
  // ... (Button Logik bleibt gleich wie vorher)
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == HIGH) {
    if (buttonTimer == 0) buttonTimer = millis(); 
    if (millis() - buttonTimer > 1500 && !cheatTriggered) {
      cheatTriggered = true;
      executeCheat();
    }
  } else {
    if (buttonTimer > 0) {
      unsigned long pressDuration = millis() - buttonTimer;
      if (pressDuration > 50 && !cheatTriggered) handleNormalClick();
      buttonTimer = 0;
      cheatTriggered = false;
    }
  }

  if (gameMode == 1) updateStacker();
  else if (gameMode == 2) updateCatcher();
  else if (gameMode == 3) updateDodge();
}

// --- PARTY HYPE FUNKTION ---
void partyAlarm() {
  u8x8.clear();
  u8x8.drawString(4, 2, "PARTY TIME!");
  u8x8.drawString(2, 4, "LEVEL COMPLETE");

  // Kurze Party-Hype Melodie mit Blaulicht-Effekt
  int melody[] = {600, 800, 600, 800, 1000, 1200, 1000, 1200};
  
  for (int i = 0; i < 3; i++) { // 3 Runden Party
    for (int note = 0; note < 8; note++) {
      digitalWrite(ledPin, HIGH); // LED AN
      tone(buzzerPin, melody[note], 100);
      delay(100);
      digitalWrite(ledPin, LOW);  // LED AUS
      delay(50);
    }
  }
}

void winGame() {
  // Erst die Herzen auf dem OLED regnen lassen
  for(int i=0; i<2; i++) oledHeartRain(false);
  
  // Dann den fetten Alarm starten!
  partyAlarm();
  
  gameMode = 1; 
  resetVariables();
}

void gameOver() {
  digitalWrite(ledPin, LOW); // Sicherstellen, dass LED aus ist
  tone(buzzerPin, 150, 600);
  for(int i=0; i<2; i++) oledHeartRain(true); 
  lc.clearDisplay(0);
  gameMode = 1; 
  resetVariables();
}

// --- RESTLICHE FUNKTIONEN (OLED Animationen, Logik etc.) ---

void oledHeartRain(bool broken) {
  u8x8.clear();
  String icon = broken ? "X " : "<3";
  for (int row = 0; row < 8; row++) {
    u8x8.clear();
    for (int i = 0; i < 4; i++) {
      u8x8.drawString(i * 4, row % 8, icon.c_str());
      u8x8.drawString((i * 4) + 2, (row + 2) % 8, icon.c_str());
    }
    if (broken) u8x8.drawString(4, 3, "GAME OVER");
    else u8x8.drawString(6, 3, "WINNER!");
    delay(100);
  }
}

void updateOLED(String msg, String extra) {
  u8x8.drawString(0, 0, "MODE:");
  if (gameMode == 1) u8x8.drawString(6, 0, "STACKER");
  else if (gameMode == 2) u8x8.drawString(6, 0, "CATCHER");
  else if (gameMode == 3) u8x8.drawString(6, 0, "DODGE   ");
  u8x8.drawString(0, 2, "SCORE:");
  char s[4]; itoa(score, s, 10); u8x8.drawString(7, 2, s);
  u8x8.clearLine(5); u8x8.drawString(0, 5, msg.c_str());
}

void resetVariables() {
  currentLevel = 0; blockWidth = 4; blockPos = 0; direction = 1;
  moveDelay = 250; score = 0; itemRow = 0; itemCol = random(0, 8);
  playerPos = 2; dodgeSpeed = 200; wallRow = 0; gapPos = random(0, 6);
  lc.clearDisplay(0);
  lastMoveTime = millis();
  u8x8.clear();
  updateOLED("GO!", "");
  digitalWrite(ledPin, LOW);
}

void handleNormalClick() {
  tone(buzzerPin, 1000, 30);
  if (gameMode == 1) checkStack();
  else if (gameMode == 2) { lc.setRow(0, 7, 0); playerPos = (playerPos + 1) % 6; }
  else if (gameMode == 3) { lc.setLed(0, 7, playerPos, false); playerPos = (playerPos + 1) % 8; }
}

void executeCheat() {
  tone(buzzerPin, 2000, 100);
  showTick((gameMode % 3) + 1);
}

void showTick(int nextMode) {
  lc.clearDisplay(0);
  lc.setLed(0, 4, 2, true); lc.setLed(0, 5, 3, true);
  lc.setLed(0, 4, 4, true); lc.setLed(0, 3, 5, true);
  lc.setLed(0, 2, 6, true);
  u8x8.clear();
  u8x8.drawString(2, 3, "LEVEL UP!");
  delay(1200);
  gameMode = nextMode;
  resetVariables();
}

void drawBlock(int row, int pos, int width) {
  byte data = 0;
  for (int i = 0; i < width; i++) data |= (1 << (7 - (pos + i)));
  lc.setRow(0, row, data);
}

void updateStacker() {
  if (millis() - lastMoveTime > moveDelay) {
    lc.setRow(0, currentLevel, 0); 
    blockPos += direction;
    if (blockPos + blockWidth >= 8 || blockPos <= 0) direction *= -1;
    drawBlock(currentLevel, blockPos, blockWidth);
    lastMoveTime = millis();
  }
}

void checkStack() {
  if (currentLevel > 0) {
    int prevP = stackPos[currentLevel - 1], prevW = stackWidth[currentLevel - 1];
    int start = max(blockPos, prevP), end = min(blockPos + blockWidth, prevP + prevW);
    int overlap = end - start;
    if (overlap <= 0) { gameOver(); return; }
    blockWidth = overlap; blockPos = start;
  }
  stackPos[currentLevel] = blockPos; stackWidth[currentLevel] = blockWidth;
  drawBlock(currentLevel, blockPos, blockWidth);
  currentLevel++; score = currentLevel;
  updateOLED("NICE!", "");
  if (currentLevel >= 8) showTick(2); 
}

void updateCatcher() {
  lc.setLed(0, 7, playerPos, true); lc.setLed(0, 7, playerPos + 1, true); lc.setLed(0, 7, playerPos + 2, true);
  if (millis() - lastMoveTime > 250) {
    lc.setLed(0, itemRow, itemCol, false);
    itemRow++;
    if (itemRow > 7) {
      if (itemCol >= playerPos && itemCol <= playerPos + 2) {
        tone(buzzerPin, 1800, 30); score++; updateOLED("GOT IT!", "");
        itemRow = 0; itemCol = random(0, 8);
      } else { gameOver(); return; }
    }
    lc.setLed(0, itemRow, itemCol, true);
    lastMoveTime = millis();
  }
  if (score >= 3) showTick(3); 
}

void updateDodge() {
  lc.setLed(0, 7, playerPos, true);
  if (millis() - lastMoveTime > 200) {
    lc.setRow(0, wallRow, 0); wallRow++;
    if (wallRow > 7) {
      if (playerPos >= gapPos && playerPos <= gapPos + 2) {
        tone(buzzerPin, 1500, 30); score++; updateOLED("DODGED!", "");
        wallRow = 0; gapPos = random(0, 6);
      } else { gameOver(); return; }
    }
    byte wallPattern = 0xFF;
    bitClear(wallPattern, 7 - gapPos); bitClear(wallPattern, 7 - (gapPos + 1)); bitClear(wallPattern, 7 - (gapPos + 2));
    lc.setRow(0, wallRow, wallPattern);
    lastMoveTime = millis();
  }
  if (score >= 3) winGame(); 
}