#include <EDB.h>

// Use the Internal Arduino EEPROM as storage
#include <EEPROM.h>

#define TABLE_SIZE 512
#define RECORDS_TO_CREATE 20

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
int axleAngle = 0;

int minVal = 0;
int maxVal = 1023;
int neutralVal = ((maxVal+minVal)/2);

void setup() {
  Serial.begin(9600);

  Serial.print("Creating table...");
  // create table at with starting address 0
  db.create(0, TABLE_SIZE, (unsigned int)sizeof(logEvent));
  Serial.println("DONE");

  createRecords(RECORDS_TO_CREATE);
  countRecords();
  selectAll();
  Serial.println("End of program");
  
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
  Serial.print("Creating Records...");
  for (int recno = 1; recno <= num_recs; recno++)
  {
    logEvent.id = recno; 
    logEvent.axleAngle = analogRead(A0);
    Serial.print("Angle stored: ");
    Serial.println(logEvent.axleAngle);
    EDB_Status result = db.appendRec(EDB_REC logEvent);
    if (result != EDB_OK) printError(result);
    else {delay(400);}
  }
  Serial.println("DONE");
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

void countRecords() {
  Serial.print("Record Count: "); 
  Serial.println(db.count());
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
