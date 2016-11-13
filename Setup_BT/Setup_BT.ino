void setup(){
  Serial.begin(38400);
  Serial.write("AT+UART=38400,0,0\r\n");
  checkResponse();
  Serial.write("AT+CLASS=0x8C080C\r\n");
  checkResponse();
  Serial.write("AT+ROLE=0\r\n");
  checkResponse();
  Serial.write("AT+NAME=Capstone\r\n");
  checkResponse();
  Serial.write("AT+PSWD=1234\r\n");
  checkResponse();
  Serial.write("AT+ROLE=0\r\n");
  checkResponse();
}

void checkResponse(){
  delay(500);
  String inputString = "";
  if(!Serial.available())
  {
     digitalWrite(13, HIGH);
     delay(1000);
     digitalWrite(13, LOW);
  }
  while (Serial.available()){
    char inChar = (char)Serial.read(); 
    inputString += inChar;
    if (inChar == '\n'){
      if(inputString == "OK\r\n"){
        Serial.print(inputString);
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
        delay(200);
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
        delay(200);
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
      }
      else
      {
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
      }
    } 
  }
  delay(1000);
}

void loop(){
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}

