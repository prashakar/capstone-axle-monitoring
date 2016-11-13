void setup() {
  Serial.begin(38400);
  //dht.begin();
  Serial.println("start");
  }

void loop(){  
  char c; 

  if(Serial.available()) {  
   c = Serial.read();  
   if(c=='t'){
    Serial.println("test");
    readSensor();
  }
}
}

void readSensor(){
  Serial.print("Humidity: ");

  Serial.print("Temperature: ");

  Serial.print("Heat index: ");
}
