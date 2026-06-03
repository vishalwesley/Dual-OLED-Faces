#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED 1 -> SDA=8 SCL=9
TwoWire I2C_1 = TwoWire(0);

// OLED 2 -> SDA=16 SCL=17
TwoWire I2C_2 = TwoWire(1);

Adafruit_SSD1306 oled1(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_1, -1);
Adafruit_SSD1306 oled2(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_2, -1);

int mouthFrame = 0;

void drawFace(Adafruit_SSD1306 &d, bool lookRight, bool talking, bool blink)
{
  d.clearDisplay();

  int eyeY = 22;

  int leftEyeX  = 40;
  int rightEyeX = 88;

  int pupilOffset = 0;

  if (lookRight)
    pupilOffset = 3;
  else
    pupilOffset = -3;

  if (blink)
  {
    d.drawLine(leftEyeX - 10, eyeY, leftEyeX + 10, eyeY, SSD1306_WHITE);
    d.drawLine(rightEyeX - 10, eyeY, rightEyeX + 10, eyeY, SSD1306_WHITE);
  }
  else
  {
    d.fillCircle(leftEyeX, eyeY, 10, SSD1306_WHITE);
    d.fillCircle(rightEyeX, eyeY, 10, SSD1306_WHITE);

    d.fillCircle(leftEyeX + pupilOffset, eyeY, 4, SSD1306_BLACK);
    d.fillCircle(rightEyeX + pupilOffset, eyeY, 4, SSD1306_BLACK);
  }

  // Mouth
  if (talking)
  {
    switch (mouthFrame)
    {
      case 0:
        d.drawLine(58, 48, 70, 48, SSD1306_WHITE);
        break;

      case 1:
        d.drawRoundRect(60, 45, 8, 5, 2, SSD1306_WHITE);
        break;

      case 2:
        d.drawRoundRect(58, 43, 12, 9, 3, SSD1306_WHITE);
        break;

      case 3:
        d.drawRect(57, 42, 14, 12, SSD1306_WHITE);
        break;

      case 4:
        d.drawRoundRect(58, 43, 12, 9, 3, SSD1306_WHITE);
        break;

      case 5:
        d.drawRoundRect(60, 45, 8, 5, 2, SSD1306_WHITE);
        break;
    }
  }
  else
  {
    d.drawLine(58, 48, 70, 48, SSD1306_WHITE);
  }

  d.display();
}

void setup()
{
  I2C_1.begin(8, 9);
  I2C_2.begin(16, 17);

  oled1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop()
{
  static unsigned long lastSwitch = 0;
  static unsigned long lastMouth = 0;

  bool phase = ((millis() / 3000) % 2 == 0);

  bool blink =
      (millis() % 5000 > 4700);

  if (millis() - lastMouth > 120)
  {
    mouthFrame++;
    if (mouthFrame > 5)
      mouthFrame = 0;

    lastMouth = millis();
  }

  if (phase)
  {
    // OLED1 talks -> looks right
    drawFace(oled1, true, true, blink);

    // OLED2 listens -> looks left
    drawFace(oled2, false, false, blink);
  }
  else
  {
    // OLED1 listens -> looks right
    drawFace(oled1, true, false, blink);

    // OLED2 talks -> looks left
    drawFace(oled2, false, true, blink);
  }

  delay(20);
}