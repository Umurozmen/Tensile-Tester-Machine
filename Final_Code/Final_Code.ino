
const int dirPin = 2;
const int stepPin = 3;
const int enPin = 4;
const int ms1Pin = 5;
const int HX711_dout = 6;
const int HX711_sck = 7; 
const int stepsPerRevolution = 200;
float turn = 0;

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup()
{
  pinMode(9, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, INPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ms1Pin, OUTPUT);
  
 Serial.begin(57600);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  float calibrationValue; 
  calibrationValue = -237923.06;
#if defined(ESP8266)|| defined(ESP32)
  
#endif

  unsigned long stabilizingtime = 2000;
  boolean _tare = true; 
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

  }

void loop()
{
    // Set motor direction clockwise
  digitalWrite(enPin, HIGH);
  digitalWrite(ms1Pin, HIGH);
 // digitalWrite(dirPin, LOW);
 
if(digitalRead(9) == HIGH){
  
 // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {  
    
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(3000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(3000);
Serial.begin(57600);

       static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData(); 
      Serial.print(turn*0.00019890, 6);
      Serial.print(",");
      Serial.print("\t");
      Serial.print(i*9.81, 3);
      
      
      newDataReady = 0;
      t = millis();
      //Serial.print(x);
Serial.print("\n");
    }
  
  }

turn = turn + 1;
}

}




  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
   }
