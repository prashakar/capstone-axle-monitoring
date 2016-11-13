#include <EDB.h>
#include <SoftwareSerial.h>

// Use the Internal Arduino EEPROM as storage
#include <EEPROM.h>

//Include the LCD Display library
#include <LiquidCrystal.h>

#define TABLE_SIZE 512
#define RECORDS_TO_CREATE 100

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

//Initialize the library with RX and TX pins
//SoftwareSerial btSerial(6,7); //RX, TX

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


//init global variables
int angleVal = 0;
int reading = 0;
int axleAngle = 0;

int minVal = 0;
int maxVal = 1023;
int neutralVal = ((maxVal+minVal)/2);

void setup() {
  //Serial.begin(9600);
  
  //Begin serial connection to bluetooth module
  Serial.begin(38400);
  //Serial.println("CONNECTED BT");
  //setup the lcd's num of columns and rows
  lcd.begin(16,2);

//  //print initialization message
//  lcd.print("Display init");

  //Serial.print("Creating table...");
  // create table at with starting address 0
  db.create(0, TABLE_SIZE, (unsigned int)sizeof(logEvent));
  //Serial.println("DONE");

  createRecords(RECORDS_TO_CREATE);
  countRecords();
  //selectAll();
  Serial.print("~");
  
}

void loop() {
  //angleVal = analogRead(A0);
  //Serial.print("Angle from sensor: ");
  //Serial.println(angleVal);

  //angleVal = angleVal - neutralVal;
  //axleAngle = angleVal/4;

  //delay(300);
}

void createRecords(int num_recs) {
  //Serial.print("Creating Records...");
  for (int recno = 1; recno <= num_recs; recno++) {
    logEvent.id = recno; 
    reading = analogRead(A0);
    logEvent.axleAngle = (reading - neutralVal)/4;
    //Serial.print("Reading: ");
    //Serial.println(reading);
    //Serial.print("Angle stored: ");
    Serial.print(logEvent.axleAngle);
//    Serial.print("&");

    //Serial.print("Angle stored: ");
    //Serial.println(logEvent.axleAngle);
    //lcd.clear();
    EDB_Status result = db.appendRec(EDB_REC logEvent);
    if (result != EDB_OK) printError(result);
    else {delay(100);}
    if (-10 < logEvent.axleAngle && logEvent.axleAngle < 10){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OK");
      lcd.noBlink();
      
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("NOK");
      lcd.setCursor(0,1);
      lcd.print("WEAR DETECTED");
      lcd.blink();
      Serial.print("*");
    }
  }
  //Serial.println("DONE");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Done");
}

void printError(EDB_Status err) {
  //Serial.print("ERROR: ");
  switch (err)
  {
    case EDB_OUT_OF_RANGE:
      //Serial.println("Recno out of range");
      break;
    case EDB_TABLE_FULL:
      //Serial.println("Table full");
      break;
    case EDB_OK:
    default:
      //Serial.println("OK");
      break;
  }
}

void countRecords() {
  //Serial.print("Record Count: "); 
  //Serial.println(db.count());
}

void selectAll() {  
  for (int recno = 1; recno <= db.count(); recno++)
  {
    EDB_Status result = db.readRec(recno, EDB_REC logEvent);
    if (result == EDB_OK)
    {
      Serial.print("index: "); 
      Serial.print(recno);
      Serial.print(" ID: "); 
      Serial.print(logEvent.id);
      Serial.print(" Angle: "); 
      Serial.println(logEvent.axleAngle);   
    }
    else printError(result);
  }
}
