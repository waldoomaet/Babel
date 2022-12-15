#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8);

void setup() {
  
SIM900.begin(19200); //Configura velocidad serial para el SIM900
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.

}

void loop() {

    SIM900.println("ATD8295636981;");
    delay(30000);
    SIM900.println("ATH");
  } 
 
