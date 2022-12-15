#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8); // Configura el puerto serial para el SIM900

int CONTACTO;

void setup() {

SIM900.begin(19200); //Configura velocidad serial para el SIM900
delay(10000); //Retardo para que encuentra a una RED
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.
pinMode(12,INPUT);
}

void loop() {

CONTACTO = digitalRead(12);

if (CONTACTO = HIGH){
 
//llamada

Serial.println("llamando");
SIM900.println("ATD8295636981;"); //llama Celular
delay(100);
SIM900.println();
delay(15000); // wait for 15 seconds...
SIM900.println("ATH"); // Cuelga el telefono
Serial.println("colgando");

//mensaje

SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
delay(100);
SIM900.println("AT+CMGS=\"8295636981\""); // recipient's mobile number, in international format
delay(100);
SIM900.println("MENSAJE DE AUXILIO"); // message to send
delay(100);
SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
delay(100);
SIM900.println();
delay(5000); // Tiempo para que se envie el mensaje
Serial.println("SMS sent successfully");
delay(15000);

}
}
