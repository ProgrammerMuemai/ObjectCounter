#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// ตั้งค่าจอ LCD (ปรับที่อยู่ I2C หากจำเป็น)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ตั้งค่าขาของเซ็นเซอร์, Buzzer และปุ่ม
const int sensorPin = 2;  // ขาเซ็นเซอร์
const int buzzerPin = 3;  // ขา Buzzer
const int button1Pin = 8; // ปุ่มเพิ่ม
const int button2Pin = 9; // ปุ่มลด
const int button3Pin = 10; // ปุ่มบันทึก

// ตัวแปรนับจำนวนและค่าคงที่
int counter = 7;
int maxCount = 7; // ค่าสูงสุดของ count
bool adjustMode = false; // โหมดปรับค่า

void setup() {
  // เริ่มต้นจอ LCD
  lcd.init();
  lcd.backlight();

  // กำหนดโหมดของขา I/O
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  // เริ่มต้นการแสดงผล
  lcd.setCursor(0, 0);
  lcd.print("Object Counter");
  updateDisplay();
}

void loop() {
  if (adjustMode) {
    // โหมดปรับค่า: รอการปรับค่าหรือบันทึก
    if (digitalRead(button1Pin) == LOW) { // ปุ่มเพิ่ม
      maxCount++;
      delay(200); // กันการกดซ้ำ
      lcd.setCursor(0, 0);
      lcd.print("Adjust Max Count ");
      lcd.setCursor(0, 1);
      lcd.print("Max: ");
      lcd.print(maxCount);
      lcd.print("     ");
      counter = maxCount; // ปรับค่า counter ให้ตรงกับ maxCount ทันที
    }

    if (digitalRead(button2Pin) == LOW) { // ปุ่มลด
      if (maxCount > 1) {
        maxCount--;
      }
      delay(200); // กันการกดซ้ำ
      lcd.setCursor(0, 0);
      lcd.print("Adjust Max Count ");
      lcd.setCursor(0, 1);
      lcd.print("Max: ");
      lcd.print(maxCount);
      lcd.print("     ");
      counter = maxCount; // ปรับค่า counter ให้ตรงกับ maxCount ทันที
    }

    if (digitalRead(button3Pin) == LOW) { // ปุ่มบันทึก
      delay(200); // กันการกดซ้ำ
      adjustMode = false;
      lcd.setCursor(0, 0);
      lcd.print("Max Count Saved  ");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("Object Counter   ");
      updateDisplay();
    }
    return; // ออกจาก loop หากอยู่ในโหมดปรับค่า
  }

  // อ่านค่าจากเซ็นเซอร์
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == LOW) { // ตรวจจับวัตถุได้เมื่อค่าเป็น LOW
    tone(buzzerPin, 1000, 200); // ส่งเสียง 1kHz เป็นเวลา 200ms

    // ลดตัวนับ
    counter--;

    // หากตัวนับถึง 0 ให้ส่งเสียง 3 ครั้งและรีเซ็ตเป็น maxCount
    if (counter < 0) {
      for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 1000, 200);
        delay(300);
      }
      counter = maxCount;
    }

    // อัปเดตการแสดงผล
    updateDisplay();

    // รอจนกว่าวัตถุจะพ้นเซ็นเซอร์
    while (digitalRead(sensorPin) == LOW) {
      delay(10);
    }
  }

  // ตรวจสอบปุ่มสำหรับเข้าสู่โหมดปรับค่า
  if (digitalRead(button1Pin) == LOW || digitalRead(button2Pin) == LOW) {
    adjustMode = true;
    lcd.setCursor(0, 0);
    lcd.print("Adjust Max Count ");
    lcd.setCursor(0, 1);
    lcd.print("Max: ");
    lcd.print(maxCount);
    lcd.print("     ");
  }
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("Count: ");
  lcd.print(counter);
  lcd.print("     "); // เคลียร์ช่องเกิน
}
