#include <LiquidCrystal.h>

const int potentiometer = A6;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int buzzer = 9;
const int led = 6;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void prank()
{
  lcd.setCursor(0, 0);
  lcd.print("ARMED");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DETONATION IN:");
  lcd.setCursor(0, 1);
  
  int sinceToneStart = 0;//Number of ms since tone started
  int numCycles = 0;//Number of 1ms cycles
  int numTones = 0;//Number of tone and LED flashes
  int nextDelay = 1000;//Interval to next tone and LED flash
  int increment = 0;//Current number of 1ms cycles to next tone and LED flash
  
  for (int i=0; i<8000; i++)
  {
    if (increment == nextDelay)
    {
      digitalWrite(led, HIGH);
      tone(buzzer, 640);
      sinceToneStart = 0;
      nextDelay = 1000*exp(-0.16*numTones);

      numTones++;
      increment = 0;
    }

    if (sinceToneStart == 50 || i == 7999)
    {
      noTone(buzzer);
      digitalWrite(led, LOW);
    }

    if (numCycles%1000 == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DETONATION IN:");
      lcd.setCursor(0, 1);
      lcd.print((8000-i)/1000.0);
      lcd.print(" sec");
    }
   
    increment++;
    numCycles++;
    sinceToneStart++;
    delay(1);
  }

  lcd.clear();
  lcd.print("BOOM!");
  delay(3000);
}

void setup() {
  pinMode(potentiometer, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);

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
  lcd.clear();
  
  prank();
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
