#include <LiquidCrystal.h>

const int potentiometer = A6;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(potentiometer, INPUT);

  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" MTE201 Caliper");
  lcd.setCursor(0, 1);
  
  for (int i=0; i<16; i++)
  {
    lcd.print("#");
    delay(65);
  }
  
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potentiometer);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(potValue);
  lcd.setCursor(0, 1);
  //lcd.print(calibration_curve(potValue));

  Serial.println(potValue);

  delay(30);
}

double calibration_curve(int potValue)
{
  return 54.3*sqrt(2.0)*sqrt(1.0-cos(0.00402*(potValue-285.0)));
}
