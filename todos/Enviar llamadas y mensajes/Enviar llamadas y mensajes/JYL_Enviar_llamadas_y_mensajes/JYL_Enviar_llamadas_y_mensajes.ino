#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8); // Configura el puerto serial para el SIM900
 
void setup()
{
SIM900.begin(19200); //Configura velocidad serial para el SIM900
delay(15000); //Retardo para que encuentra a una RED
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.
}
 
void loop()

{
//llamada

Serial.println("llamando");
SIM900.println("ATD8295636981;"); //Celular
delay(100);
SIM900.println();
delay(15000); // wait for 15 seconds...
SIM900.println("ATH"); // Cuelga el telefono
Serial.println("colgando");

}
 

