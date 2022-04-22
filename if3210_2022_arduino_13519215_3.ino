#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>

unsigned long myTime;

int x = 0;
int position = 0;
int position_open = 90;
int position_close = 0;
int i = 0;
int j = 0;
int a = 0, b = 0, c = 0, d = 0;
int var = 0;
int password[] = {1, 2, 3, 4};
int Ctemp[] = {10, 10, 10, 10};
char f = '*';

const byte numRows = 4; // number of rows on the keypad
const byte numCols = 4; // number of columns on the keypad

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

// Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {7, 6, 5, 4};   // Rows 0 to 3
byte colPins[numCols] = {3, 2, A0, A1}; // Columns 0 to 3

// initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
  Wire.begin(); // init wire
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  position = 0;
  char prompt = myKeypad.getKey();
  if (prompt == 'A')
  {
    myTime = millis();
    bool correct = false;
    while ((millis() - myTime) / 1000 < 15 && !correct)
    {
      lcd.setCursor(0, 0);
      lcd.print(" Sisa Waktu: ");
      lcd.print(15 - (millis() - myTime) / 1000);
      lcd.print("       ");
      char keypressed = myKeypad.getKey();
      if (keypressed)
      {
        lcd.setCursor(6 + var, 1);
        lcd.print(keypressed), lcd.setCursor(6 + var, 1), lcd.print(f);
        keypressed = keypressed - 48;
        var++;
        switch (var)
        {
        case 1:
          a = keypressed;
          break;
        case 2:
          b = keypressed;
          break;
        case 3:
          c = keypressed;
          break;
        case 4:
          d = keypressed;
          delay(50);
          if (a == password[0] && b == password[1] && c == password[2] && d == password[3])
          {
            lcd.clear();
            var = 0;
            correct = true;
          }
          var = 0;
          lcd.clear();
          break;
        }
      }
    }
    if (correct)
    {
      // Bisa Ganti Password
      int var_temp = 0;
      bool correct2 = false;
      while (Ctemp[0] == 10 || Ctemp[1] == 10 || Ctemp[2] == 10 || Ctemp[3] == 10 && !correct2)
      {
        lcd.setCursor(0, 0);
        lcd.print("Sandi Baru:");
        char keypressed2 = myKeypad.getKey();
        if (keypressed2)
        {
          lcd.setCursor(6 + var_temp, 1);
          lcd.print(keypressed2), lcd.setCursor(6 + var_temp, 1), lcd.print(f);
          keypressed2 = keypressed2 - 48;
          var_temp++;
          switch (var_temp)
          {
          case 1:
            Ctemp[0] = keypressed2;
            break;
          case 2:
            Ctemp[1] = keypressed2;
            break;
          case 3:
            Ctemp[2] = keypressed2;
            break;
          case 4:
            Ctemp[3] = keypressed2;
            Wire.beginTransmission(3); // address is queued for checking if the slave is present
            for (int i = 0; i < 4; i++)
            {
              password[i] = Ctemp[i];
              Wire.write(password[i]); //  kirim ke arduino 1
            }
            Wire.endTransmission();
            delay(50);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Sandi Berubah!");
            delay(1000);
            correct2 = true;
            var_temp = 0;
            lcd.clear();
          }
        }
      }
      // udah berubah
      for (int i = 0; i < 4; i++)
      {
        // set lagi ke awal
        Ctemp[i] = 10;
      }
    }
    // PASSWORD SALAH
    if (!correct)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD SALAH");
      lcd.setCursor(0, 1);
      lcd.print("           ");
      delay(1000);
      lcd.clear();
      for (i = 10; i >= 0; i--)
      {
        lcd.setCursor(0, 0);
        lcd.print("PASSWORD SALAH");
        lcd.setCursor(0, 1);
        lcd.print("           ");
        lcd.setCursor(0, 1);
        lcd.print("Coba lagi: ");
        lcd.print(i);
        lcd.print("       ");
        delay(1000);
      }
      lcd.clear();
    }
  }
  if (!prompt)
  {
    lcd.setCursor(0, 0), lcd.print("Setting Password");
    // lcd.setCursor(0, 0), lcd.print("Setting Password");
  }
  delay(200);
}