#include <LiquidCrystal.h>

const int potentiometer = A6;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(potentiometer, INPUT);

  lcd.begin(16, 2);
  
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potentiometer);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(potValue);

  Serial.println(potValue);

  delay(50);
}
