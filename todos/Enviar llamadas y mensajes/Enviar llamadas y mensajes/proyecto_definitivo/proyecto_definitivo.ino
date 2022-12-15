#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8); // Configura el puerto serial para el SIM900

int Pulso = 12;

void setup() {

SIM900.begin(19200); //Configura velocidad serial para el SIM900
delay(25000); //Retardo para que encuentra a una RED
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.

pinMode (Pulso,INPUT);

}

void loop() {

if (digitalRead(Pulso) == HIGH) {
  
 Serial.println("llamando");
SIM900.println("ATD8295636981;"); //Celular
delay(100);
SIM900.println();
delay(30000); // wait for 10 seconds...
SIM900.println("ATH"); // Cuelta el telefono
Serial.println("colgando");

  } 

  delay(1000);
}
