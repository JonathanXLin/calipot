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

byte digits = 1;

byte uncertaintySymbol[8] = {
  B00100,
  B00100,
  B11111,
  B00100,
  B00100,
  B00000,
  B11111,
};

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

  lcd.createChar(0, uncertaintySymbol); 
  
  //prank();
}

void loop() {
  int potValue = analogRead(potentiometer);

  lcd.clear();
  lcd.setCursor(0, 0);

  double val = calibration_curve(potValue);
  if (val == -1)
  {
    lcd.print("ERROR:");
    lcd.setCursor(0, 1);
    lcd.print("Exceeds Bounds");
  }
  else
  {
    lcd.print(val, digits);
    lcd.print(" mm ");  
    lcd.write(byte(0));
    lcd.print(" 0.8 mm");
  }

  Serial.println(val);
  
  delay(30);
}

double calibration_curve(int potValue)//Rounded to nearest integer
{
  double val = 54*sqrt(2.0)*sqrt(1.0-cos(0.2335*(potValue-289.1)*3.1416/180.0));

  if (potValue <= 289)
    return 0.0;
  else if (val <= 101)
    return val;
  else//Error case, exceeds 180 degrees
    return -1;
}
