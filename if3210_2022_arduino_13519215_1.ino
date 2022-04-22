#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>

unsigned long myTime;
Servo s;

int x = 0;
int servo_state;
int position = 0;
int position_open = 90;
int position_close = 0;
int i = 0;
int j = 0;
int a = 0, b = 0, c = 0, d = 0;
int var = 0;
// int C1 = 1, C2 = 2, C3 = 3, C4 = 4;
int password[] = {1, 2, 3, 4};
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
byte rowPins[numRows] = {7, 6, 5, 4};     // Rows 0 to 3
byte colPins[numCols] = {A0, A1, A2, A3}; // Columns 0 to 3

// initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
  Wire.begin(3); // init wire
  Wire.onReceive(receiveEvent);
  lcd.begin(16, 2);
  pinMode(2, OUTPUT); // LED Green
  s.attach(3, 500, 2500);
  Serial.begin(9600);
}

void receiveEvent(int howMany)
{
  if (howMany == 1)
  {
    x = Wire.read(); // read one character from the I2C
                     // Cek apakah tombol dari dalam ditekan atau tidak
  }
  else
  {
    for (int i = 0; i < howMany; i++)
    {
      password[i] = Wire.read();
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sandi Berubah!");
    delay(1000);
    lcd.clear();
  }
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
            lcd.setCursor(4, 0);
            lcd.print("UNLOCKED");
            lcd.setCursor(3, 1);
            if (servo_state == 0)
            {
              // lcd.print("Terbuka");
              s.write(position_open);
              servo_state = 1;
              x = 1;
              Wire.beginTransmission(13);
              Wire.write(x); // sends x
              // Serial.println(x);
              Wire.endTransmission(); // stop transmitting
              myTime = millis();
            }
            else
            {
              // lcd.print("Tertutup");
              s.write(position_close);
              servo_state = 0;
            }
            digitalWrite(2, HIGH);
            delay(1000);
            lcd.clear();
            digitalWrite(2, LOW);
            var = 0;
            lcd.clear();
            correct = true;
          }
          var = 0;
          lcd.clear();
          break;
        }
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
    if (x == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0), lcd.print("Pintu Terbuka");
      lcd.setCursor(0, 1), lcd.print("             ");
      s.write(position_open);
      servo_state = 1;
      delay(1000);
    }
    else
    {
      s.write(position_close);
      servo_state = 0;
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0), lcd.print("Welcome to Lab");
    }
    // lcd.setCursor(0, 0), lcd.print("Welcome to Lab");
  }
  delay(200);
  // servo
  // for (position = 1; position <= 179; position += 1) {
  // s.write(position);
  // delay(20); // Wait for 20 millisecond(s)
  //}
  // for (position = 179; position >= 1; position -= 1) {
  // s.write(position);
  //}
}