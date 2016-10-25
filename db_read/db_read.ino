#include <EDB.h>

// Use the Internal Arduino EEPROM as storage
#include <EEPROM.h>

#define TABLE_SIZE 512
#define RECORDS_TO_CREATE 10

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

void setup() {
    Serial.begin(9600);
    Serial.println("Reading from db on EEPROM...");
    selectAll();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void selectAll() {  
  db.open(0);
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
