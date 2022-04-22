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
int C1 = 1, C2 = 2, C3 = 3, C4 = 4;
int Ctemp1 = 0, Ctemp1 = 0, Ctemp1 = 0, Ctemp1 = 0;
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
byte colPins[numCols] = {3, 2, A0, A1}; // Columns 0 to 3

// initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
  Wire.begin(); // init wire
  Wire.onReceive(receiveEvent);
  lcd.begin(16, 2);
  s.attach(3, 500, 2500);
  Serial.begin(9600);
}

void receiveEvent( int howMany )
{
   x = Wire.read(); // read one character from the I2C
   // Cek apakah tombol dari dalam ditekan atau tidak
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
          if (a == C1 && b == C2 && c == C3 && d == C4)
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
    if (correct){
      //Bisa Ganti Password
      int var_temp = 0;
      while (Ctemp1 == 0 && Ctemp2 == 0 && Ctemp3 == 0 && Ctemp4 == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print("Sandi Baru:");
        char keypressed = myKeypad.getKey();
        if (keypressed)
        {
          lcd.setCursor(6 + var, 1);
          lcd.print(keypressed), lcd.setCursor(6 + var, 1), lcd.print(f);
          keypressed = keypressed - 48;
          var_temp++;
          switch (var_temp)
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
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Sandi Berubah!");
            Wire.beginTransmission(3);
            Wire.write(x); // sends x
            //Serial.println(x);
            Wire.endTransmission(); // stop transmitting
            delay(1000);
            var_temp = 0;
            lcd.clear();
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
       if (x == 1) {
         lcd.clear();
         lcd.setCursor(0, 0), lcd.print("Sedang Dipakai");
         lcd.setCursor(0, 1), lcd.print("Harap tunggu");
         s.write(position_open);
         servo_state = 1;
         delay(1000);
       }
       else{
         s.write(position_close);
         servo_state = 0;
         delay(1000);
         lcd.clear();
         lcd.setCursor(0, 0), lcd.print("Setting Password");
      }
      //lcd.setCursor(0, 0), lcd.print("Setting Password");
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