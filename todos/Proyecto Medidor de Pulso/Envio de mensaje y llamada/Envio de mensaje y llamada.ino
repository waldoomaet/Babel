#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8); // Configura el puerto serial para el SIM900
 
void setup()
{
SIM900.begin(19200); //Configura velocidad serial para el SIM900
delay(25000); //Retardo para que encuentra a una RED
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.
}
 
void loop()

{
SIM900.println("ATD8295636981;"); //Celular
delay(100);
SIM900.println();
delay(30000); // wait for 30 seconds...
SIM900.println("ATH"); // Cuelta el telefono
delay(1000);

SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
delay(100);
SIM900.println("AT+CMGS=\"8295636981\""); // recipient's mobile number, in international format
delay(100);
SIM900.println("menzaje de prueba"); // message to send
delay(100);
SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
delay(100);
SIM900.println();
delay(5000); // Tiempo para que se envie el mensaje
Serial.println("SMS sent successfully");
delay(30000);
}
 

