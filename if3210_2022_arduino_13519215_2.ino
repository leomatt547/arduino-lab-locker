// C++ code
//
#include <LiquidCrystal.h>
#include <Wire.h>    

LiquidCrystal lcd(12,11,5,4,3,2); 

unsigned long myTime;

int x =0;
int slaveAddress = 3; // Address servo di arduino 1
const int buttonPin = 13; // the number of the pushbutton pin
int buttonState = 0; // variable for reading the pushbutton status

const unsigned int NOTE_C4 = 262;
const unsigned int NOTE_E4 = 330;
const unsigned int NOTE_G4 = 392;

const int BULB_PIN = 10; // Posisi Sensor 1
const int PIR1_PIN = 9; // Posisi Sensor 1
const int PIR2_PIN = 8; // Posisi Sensor 2
const int PIEZO_PIN = 7; // Posisi Speaker
const bool SERIAL_PRINT = true;

int visitors = 0;
int lastRIPdetected = 0;
bool b_PIR1_active = false;
bool b_PIR2_active = false;

void setup()
{
  lcd.begin(16,2);

  pinMode(PIR1_PIN, INPUT); //PIR Sensor
  pinMode(PIR2_PIN, INPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(BULB_PIN, OUTPUT); // Light Bulb
  
  Wire.begin(13); // join i2c bus (address optional for master)
  pinMode( buttonPin, INPUT ); // initialize the pushbutton pin as an input
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); 
  DisplayMsg("Visitor counter", "Welcome", SERIAL_PRINT); 
}

void receiveEvent( int howMany )
{
   x = Wire.read(); // read one character from the I2C
   // Cek apakah password benar dimasukkan dari arduino 1
   myTime = millis(); // set timeout 10 detik
}

// -------- Message display on LCD and Serial (optional) ------
void DisplayMsg(String s1, String s2, bool ab_serial_print) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(s1);

  lcd.setCursor(0, 1);  
  lcd.print(s2);
  
  if (ab_serial_print) 
    Serial.println(s1 + ". " + s2);
} 


// -------- Increase/Decrease visitors ---------
void UpdateVisitorsCounter(int x){ 
int duration=100;
  
  visitors = visitors + x;
  
  lastRIPdetected = 0;  // reset detected PIR
  
  if (x>0)
    PlayNote(NOTE_G4, duration);
  else
    PlayNote(NOTE_C4, duration);  	
}

// -------- Play note ---------
void PlayNote(int note, int duration) {
  tone(PIEZO_PIN, note, duration);
  delay(duration * 1.3);
  noTone(PIEZO_PIN);
}


void loop()
{
  if(visitors > 0){
    digitalWrite(BULB_PIN, HIGH);
  }else{
    digitalWrite(BULB_PIN, LOW);
  }
  // ----------- check PIR1 ----------------
  if (digitalRead(PIR1_PIN) == HIGH) {
    
    if ( !b_PIR1_active ) {
      b_PIR1_active = true;
      
      if (lastRIPdetected == 0 && !b_PIR2_active) {  // new start
        lastRIPdetected = 1;    
 
        DisplayMsg("In PIR1", "Visit started", SERIAL_PRINT);

      } else if (lastRIPdetected == 2) { // if we were in PIR2 before
        UpdateVisitorsCounter(-1);  
        // tahan pintu 10 detik ke depan
        myTime = millis();
      
        DisplayMsg("Visitor exited", "Visitors:" + String(visitors), SERIAL_PRINT);
      } 
    }
  }else {
    b_PIR1_active = false ;  // reenable PIR1
  }
  
  
  // ----------- check PIR2 ----------------  
  if (digitalRead(PIR2_PIN) == HIGH ) {
    if ( !b_PIR2_active ) {
      b_PIR2_active = true;
      
      if (lastRIPdetected == 0 && !b_PIR1_active) { // new start
        if (visitors > 0) {
          lastRIPdetected = 2;   

          DisplayMsg("In PIR2", "Exit started", SERIAL_PRINT);
        } else 
          DisplayMsg("No more visitors", "to exit", SERIAL_PRINT);
                
      } else if (lastRIPdetected == 1) {  // if we were in PIR1 before
        UpdateVisitorsCounter(1);  
        // tahan pintu 10 detik ke depan
        myTime = millis();

        DisplayMsg("Visitor entered", "Visitors:" + String(visitors), SERIAL_PRINT);
      }
    }
  } else {
    	b_PIR2_active = false; // reenable PIR2
  }
  buttonState = digitalRead( buttonPin );
  if ( buttonState == HIGH ){
    if(x == 0){
      x = 1;
      myTime = millis();
    }
  }
  
  if((x==1) && (millis() - myTime)/1000 > 10){
    x = 0;
  }
  //Wire.beginTransmission(slaveAddress); // transmit to device #3
  Wire.beginTransmission(3);
  Wire.write(x); // sends x
  Serial.println(x);
  Wire.endTransmission(); // stop transmitting
  delay(100);
}