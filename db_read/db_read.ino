#include <EDB.h>

// Use the Internal Arduino EEPROM as storage
#include <EEPROM.h>

//Include the LCD Display library
#include <LiquidCrystal.h>

#define TABLE_SIZE 512
#define RECORDS_TO_CREATE 60

/*
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */
//Initialize the library with num of interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

struct LogEvent {
  int id;
  int axleAngle;
} 
logEvent;
// The read and write handlers for using the EEPROM Library
void writer(unsigned long address, byte data)
{
  EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
  return EEPROM.read(address);
}

// Create an EDB object with the appropriate write and read handlers
EDB db(&writer, &reader);

//threshold for wear
int wearThreshold = 10;
int diffVal = 0;

//counter for detecting amount of wear
int wearCounter = 0;

//warning threshold
int wearWarnThreshold = 17;

//error threshold
int wearErrorThreshold = 30;

void setup() {
    Serial.begin(9600);
    lcd.begin(16,2);
    lcd.clear();
    Serial.println("Reading from db on EEPROM...");
    selectAll();
    //delay(7000);
    
}

void loop() {
  // put your main code here, to run repeatedly:

}

void selectAll() {  
  db.open(0);
  for (int recno = 1; recno <= db.count(); recno++) {
    EDB_Status result = db.readRec(recno, EDB_REC logEvent);
    if (result == EDB_OK) {
      Serial.print("index: "); 
      Serial.print(recno);
      Serial.print(" ID: "); 
      Serial.print(logEvent.id);
      Serial.print(" Angle: "); 
      Serial.println(logEvent.axleAngle);
      diffVal = logEvent.axleAngle - wearThreshold;
      if (logEvent.axleAngle > 0 && (logEvent.axleAngle - wearThreshold > 0) || 
      logEvent.axleAngle < 0 && (wearThreshold + logEvent.axleAngle < 0)){
        Serial.println("EXCEEDED WEAR THRESHOLD");
        wearCounter = wearCounter + 1;
        Serial.print("Number of times exceeded threshold: ");
        Serial.println(wearCounter);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Wear counter: ");
        lcd.setCursor(14,0);
        lcd.print(wearCounter);
      }
    }
    else printError(result);
  }
  printLcdDisplay();
}

void printLcdDisplay(){
    lcd.setCursor(0,1);
    if (wearCounter > wearErrorThreshold){
      lcd.print("Critical wear");
    } else if (wearCounter > wearWarnThreshold){
      lcd.print("Wear Warning");
    } else {
      lcd.print("Systems OK");
    }
}
void printError(EDB_Status err) {
  Serial.print("ERROR: ");
  switch (err)
  {
    case EDB_OUT_OF_RANGE:
      Serial.println("Recno out of range");
      break;
    case EDB_TABLE_FULL:
      Serial.println("Table full");
      break;
    case EDB_OK:
    default:
      Serial.println("OK");
      break;
  }
}
