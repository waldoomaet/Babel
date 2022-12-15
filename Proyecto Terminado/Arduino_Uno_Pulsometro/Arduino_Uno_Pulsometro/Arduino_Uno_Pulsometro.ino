
//VARIABLES:  
volatile int Transcurrido_Array[10];             // Arreglo para registrar el tiempo transcurrido entre cada pulso(IBI).
volatile unsigned long Respaldo_Conteo = 0;      // Contador de respaldo   
volatile unsigned long Momento_UltimoPulso = 0;  // Tiempo transcurrido hasta el ultimo pulso. 
volatile int P =512;                             // Punto mas alto de una onda de pulso.
volatile int T = 512;                            // Punto inicial del pico de onda.
volatile int ValorMedioREF = 530;                // Punto medio del pico de onda.
volatile int AnchoOndaValido = 0;                // Ancho del pico de onda.
volatile boolean Primer_Pulso = true;            // Primera porcion de pulso para el calculo del BPM (arreglo). 
volatile boolean Segundo_Pulso = false;          // Segunda porcion de pulso para el registro del BPM (arreglo). 
int Pin_Entrada_sensor = 0;                      // Pin A0, donde se recibe la señal del sensor.
int Pin_Desvanecimiento_LED_BUZZER = 13;         // Pin 13, donde emite un sonido o luz en cada pulso.
// Volatile Variables, es un tipo variable que cuando ya es usada se desecha el valor que contine y toma uno nuevo. 
// Mediante volatile se actualizan los datos tomados
volatile boolean Q_S = false;
volatile int BPM;                                // Pulsos por Minuto se actualiza constantemente.                                             
volatile int signal_sensor;                      // Almacena todos los datos enviados por el sensor.
volatile int IBI = 600;                          // Almacena el tiempo entre cada de pulso (Valor 600 de referencia).
volatile boolean Pulso = false;                  // Para la deteccion de pulso valido.

void setup() { //setup() es la parte encargada de recoger la configuración.

pinMode (12,OUTPUT);

pinMode(Pin_Desvanecimiento_LED_BUZZER,OUTPUT); // Declaramos el LED o Buzzer que se acciona por cada pulso.
Serial.begin(9600);                             // Velocidad muestreo o toma de datos del arduino 
interruptSetup();                               // Metodo que ejecuta la configuracion inicial del interrupt. 
                         
}
void interruptSetup(){                           //CONFIGURACION 
  
  TCCR2A = 2;    // Desactiva la modulacion de onda en los pines 3 y 11, y va al CTC modo
  TCCR2B = 6;    // no forzar la comparacio preescalar a 256
  OCR2A = 124;   // Cantidad de ciclos 124 a 500Mhz 
  TIMSK2 = 2;    // ACTIVA LAS PAUSA  DEL MARCADOR ENTRE EL (timer)  
  sei();         // DEFINE GLOBALMENTE LOS PARAMETROS ESTALECIDOS ARRIBA INICIANDOSE
}
void loop() {     // Metodo principal que se ejecuta varias veces.
                  // NOTA! ESTE SE EJECUTA SIMULTANEAMENTE CON EL INTERRUPT.
   serialOutput();// Metodo de salida (muestra los valores finales).                                
   delay(1.67); } // Tiempo de espera en mili-segundos.
    
   
  void serialOutput(){  
   Serial.print("E");  // "E" indica el inicio de valores en la app.
   Serial.print(BPM);  // Valor de pulsos por minuto.
   Serial.print(",");  // "," Separacion de un dato en la app y monitor Serie.
   Serial.print(IBI);  // Tiempo transcurrido entre pulsos(ms).
   Serial.print(",");  // "," Separacion de un dato en la app y monitor Serie.
   Serial.println(signal_sensor);
              }   
// Rutina de servicio de interrupcion:ISR.  
ISR(TIMER2_COMPA_vect){                  
  cli();                                         // Detiene el servicio de interrupcion de rutina (ISR).                                                                                                                                               
  signal_sensor= analogRead(Pin_Entrada_sensor); // Lectura del valor arrojado por el sensor.
  Respaldo_Conteo += 2;                          // Lleva un conteo auxiliar acumulado a 2 por ciclo.
  int N = Respaldo_Conteo - Momento_UltimoPulso; // "N" registra el tiempo despues de el ultimo pulso. 
  
  // encuetra un pico optimo donte empezar los calculos del pulso  cardiaco. 
  
  if(signal_sensor< ValorMedioREF && N > (IBI/5)*3){   // Evita los falsos latidos o latidos no validos. 
    if (signal_sensor< T){                             // SI la lectura del pulso es mayor a 512.                                              
        T = signal_sensor;                             // Pulso de pico mas bajo. 
    }
  }
  if(signal_sensor> ValorMedioREF && signal_sensor> P){ // Condicion para captar picos superiores (por encima de 530). 
                                                                
    P = signal_sensor;                                  // P es el punto mas alto.
  }                                             
 
  if (N > 250){       // Condicion cuando han transcurrido mas de 250 ms.                 
    if ( (signal_sensor> ValorMedioREF) && (Pulso == false) && (N > (IBI/5)*3) ){
      Pulso = true;                                        // Se activa para posibles pulsos. 
      digitalWrite(Pin_Desvanecimiento_LED_BUZZER,HIGH);   // Activa el pin 13.
      IBI = Respaldo_Conteo - Momento_UltimoPulso;         // Medida  entre pico a pico de un pulso en ms. 
      Momento_UltimoPulso = Respaldo_Conteo;               //Guarda el ultimo pulso en otra variale.
      if(Segundo_Pulso){                        // Evalua si se registro un pulso antes (si ya paso por el primero)
        Segundo_Pulso = false;                  // Pone el valor en falso para evaluar nuevamente la condicion. 
        for(int i=0; i<=9; i++){                // Toma todos los 10 valores validos de IBI.  
                                                // los valores de IBI guardados son referencia para el BPM.
                                                // IBI esta medido en ms  y los almacena en una variable.
          Transcurrido_Array[i] = IBI;          // Guarda el valor de los IBI obtenidos.
        }
      }
      if(Primer_Pulso){                         // Evalua el primer pulso y prepara el segundo pulso. 
        Primer_Pulso = false;                   // Cambiamos a false para que evalue nuevamente la condicion.
        Segundo_Pulso = true;                   // Da aviso que el pulso siguiente es el segundo.
        sei();                                  // Reanuda la interrupciones.
      }
      word  runningTotal = 0;                   // Guarda la sumatoria de todos los valores del IBI ms.                

      for(int i=0; i<=8; i++){                              // Ciclo para filtrar los datos de "Transcurrido_Array".
        Transcurrido_Array[i] = Transcurrido_Array[i+1];    // Organizamos el arreglo para dejar solo los valores actuales.
        runningTotal += Transcurrido_Array[i];              // Suma de IBI contenidos en las primeras 8 posiciones del arreglo.
      }

      Transcurrido_Array[9] = IBI;                          // Ingresa el ultimo valor de IBI.
      runningTotal += Transcurrido_Array[9];                // Agrega el ultimo IBI a runningTotal.
      runningTotal /= 10;                                   // Promedio de los ultimos 10 IBI.
      BPM = 60000/runningTotal;                             // Cantidad de pulsaciones por minuto.  

   }


}
  if ((signal_sensor < ValorMedioREF) && (Pulso == true)){ // Cuando capture el final de un pulso CARDIACO valido. 
    digitalWrite(Pin_Desvanecimiento_LED_BUZZER,LOW);      // Apaga el pin 13 LED (PRENDE Y APAGA SEGUN EL PULSO).
    Pulso = false;                                         // Pone el pulso en "false" por que ya esta en el punto final del pulso (S)
    AnchoOndaValido = P - T;                               // Mide la distancia que existe eNtre el puNto del pulso mas alto y el puNto mas bajo de la lectura de pulso 
    ValorMedioREF = AnchoOndaValido/2 + T;                 // una pendiente cuya longitud determina la altura de la onda
                                                                         
    P = ValorMedioREF;       // Actualiza valores.
    T = ValorMedioREF;
  }

  if (N > 2500){            // Si han pasado 2.5 segundos y no hay un pulso, tome valores iniciales
                                            
    ValorMedioREF = 530;                         // ValorMedioREF POR DEFECTO EL MISMO VALOR De INICIO
    P = 512;                                    // VARIABLE P Y T  a  valor 512  como al inicio
    T = 512;                                    
    Momento_UltimoPulso = Respaldo_Conteo;      // Llama el ultimo pulso que exista del contador de muestra
    Primer_Pulso = true;                        // El primer pulso toma el valor inicial por defecto
    Segundo_Pulso = false;                      // El segundo pulso toma el valor inicial por defecto 
  }
   // FIN DEL  METODO DE CUANTIFICACION DE DATOS VALIDOS

sei();       // Inicia nuevamente las Interrupciones.

if (BPM > 100){

digitalWrite (12,HIGH);

}
}

