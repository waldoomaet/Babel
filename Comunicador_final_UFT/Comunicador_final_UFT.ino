#include <DFPlayer_Mini_Mp3.h>
//#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>

UTFTGLUE myGLCD(0x9488,A2,A1,A3,A4,A0);

#if !defined(SmallFont)
extern uint8_t SmallFont[];   
#endif

#define FONA_RX 22
#define FONA_TX 52
#define FONA_RST 23

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

//LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
SoftwareSerial mySerial(24, 25); // RX, TX

String Mensaje[] = {"Seleccione una carpeta y luego escoja el mensaje que desea mostrar...","C/","Afirmacion","Negacion","Estado","Numeros","Maldiciones","Mensaje personalizado","Numero personalizado"};
String MesNew[5][10] = {{"Si","Por supuesto","Claro","Efectivamente","Sin duda alguna"},{"No","Tampoco","Nunca","Jamas"},{"Hambre","Sed","Sueno","Dolor","Orinar","Excretar"},{"Cero","Uno","Dos","Tres","Cuatro","Cinco","Seis","Siete","Ocho","Nueve"},{"Maldita sea","Jodete","Maricon"}};
char Alfabeto[50][2] = {{' ',0},{'A',0},{'B',0},{'C',0},{'D',0},{'E',0},{'F',0},{'G',0},{'H',0},{'I',0},{'J',0},{'K',0},{'L',0},{'M',0},{'N',0},{'O',0},{'P',0},{'Q',0},{'R',0},{'S',0},{'T',0},{'U',0},{'V',0},{'W',0},{'X',0},{'Y',0},{'Z',0},{',',0},{'.',0},{'!',0},{'?',0},{'0',0},{'1',0},{'2',0},{'3',0},{'4',0},{'5',0},{'6',0},{'7',0},{'8',0},{'9',0}};
char Num0[32] = {"8497639189"};
char Num1[32] = {"8096678617"};
char Num2[32] = {"8294554731"};
char Num3[32] = {"8296374062"};
char NewNum[32] = {"Nada"};
unsigned long Time;
unsigned long NewTime;
int ArrayNumber[] = {5,9,15,25,28};
int AmountMesNew[] = {4,3,5,9,2};
int AlfabetoLong = 40;
int AmountMensaje = 8;
int MenLon = Mensaje[0].length();
int MenNum,MenNum1,Control,Control2,Control3,Control4,Control6,Control7,Control8,Control9,Control10,Control11,Control12,Musica,Reset = 0;
int ArribaMax = 950;
int ArribaMin = 796;
int OkMax = 795;
int OkMin = 715;
int AbajoMax = 714;
int AbajoMin = 581;
int BorrarMax = 580;
int BorrarMin = 515;
int LlamarMax = 514;
int LlamarMin = 151;
int AtrasMax = 150;
int Control5 = 1;
int Control1 = 16;
int ValorAng;
int CaracterI = 0;
int CaracterF = 18;
//int EnergiaMp3 = 5;
//int SiguienteMp3 = 6;
//int Pausa = 7;
//int Arriba = 8;
//int Abajo = 9;
//int Ok = 7;
//int Atras = 11;
//int Borrar = 12;
//int Call = 13;

void setup() 
{
  //lcd.begin(16, 2);
  Serial.begin(9600);
  //--------------------------------------------------
  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (21);
  //---------------------------------------------------
  randomSeed(analogRead(5));   //.kbv Due does not like A0
  pinMode(A0, OUTPUT);       //.kbv mcufriend have RD on A0
  digitalWrite(A0, HIGH);
  myGLCD.InitLCD();
  //-------------------------------------------------------
  fonaSS.begin(9600); // if you're using software serial
  
  Time = millis();
  
  if (! fona.begin(fonaSS)) {
    while (1);
  }
  // Check we connect to the network
  while (fona.getNetworkStatus() != 1 && Control11 == 0) 
  {
    NewTime = millis();
    if(Control == 0)
      {
       myGLCD.clrScr();
       myGLCD.fillScr(0,0,153);
       myGLCD.setTextSize(4);
       myGLCD.setColor(255, 0, 0);
       myGLCD.setBackColor(0,0,153);
       myGLCD.print(F("Comunicador"), 2,6);
       myGLCD.setColor(0, 255, 0);
       myGLCD.print(F("Babel"),265,6);
       myGLCD.drawLine(0,42,480,42);
       myGLCD.setTextSize(3);
       myGLCD.setColor(255,255,153);
       myGLCD.print(F("Cargando..."),LEFT,77);
       Control++;
      }
    if (NewTime - Time > 25000)
      {
        myGLCD.setColor(0,0,153);
        myGLCD.fillRect(0,50,480,190);
        myGLCD.setColor(255,255,153);
        myGLCD.print(F("Fona no responde!     Modo sin conexion!"),LEFT,77);
        Control11 = 1;
        Control9 = 1;
        delay(2800);
      }
    delay(100);
  }
  // set to external mic & headphone
  fona.setAudio(FONA_EXTAUDIO);
//----------------------------------------------------------------------------
  //pinMode(Arriba,INPUT);
  //pinMode(Abajo,INPUT);
  //pinMode(Ok,INPUT);
  //pinMode(Atras,INPUT);
  //pinMode(Borrar,INPUT);
  //pinMode(EnergiaMp3,OUTPUT);
  //pinMode(SiguienteMp3,OUTPUT);
  //pinMode(Pausa,OUTPUT);
  //pinMode(13,OUTPUT);
  pinMode(A15,INPUT_PULLUP);
  pinMode(31,INPUT_PULLUP);
  pinMode(33,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  pinMode(37,INPUT_PULLUP);
  pinMode(39,INPUT_PULLUP);
  pinMode(41,INPUT_PULLUP);
  /*digitalWrite(SiguienteMp3,LOW); //Todos los pines de control apagados
  digitalWrite(Pausa,LOW);
  //-------------------------------------
  digitalWrite(EnergiaMp3,HIGH); //Encendido del modulo MP3
  delay(2000);
  //-------------------------------------
  digitalWrite(Pausa,HIGH); //Como inicia reproduciendo, lo pone en pausa
  delay(70);
  digitalWrite(Pausa,LOW);
  if(EEPROM.read(1)==1)
    {
      EEPROM.write(0,1);
      EEPROM.write(1,0);
      Serial.println("OK");
    }*/
}

void loop()
{
  if(Control9 == 0)
    {
      myGLCD.clrScr();
      myGLCD.fillScr(0,0,153);
      myGLCD.setTextSize(4);
      myGLCD.setColor(255, 0, 0);
      myGLCD.setBackColor(0,0,153);
      myGLCD.print(F("Comunicador"), 2,6);
      myGLCD.setColor(0, 255, 0);
      myGLCD.print(F("Babel"),265,6);
      myGLCD.drawLine(0,42,480,42);
    }
  myGLCD.setColor(0,0,153);
  myGLCD.fillRect(0,50,480,190);
  myGLCD.setTextSize(2);
  myGLCD.setColor(255,255,153);
  myGLCD.print(Mensaje[MenNum],LEFT,77);
  delay(2500);
  MenNum = 2;
  //----------------------------------------------------------------------------
  while(1)                                                        //Presentacion del mensaje de inicio
    {
      myGLCD.setColor(0,0,153);
      myGLCD.fillRect(0,50,480,190);
      myGLCD.setTextSize(3);
      myGLCD.setColor(204,204,204);
      myGLCD.setBackColor(0,0,153);
      myGLCD.print(F("C:/"),LEFT,75);
      myGLCD.print(Mensaje[MenNum],60,75);
      Control = 0;
      while(Control == 0)
        {
          if(analogRead(A15) < ArribaMax && analogRead(A15) > ArribaMin || digitalRead(31) == 0)
            {
              Serial.println(analogRead(A15));
              Control = 1;
              MenNum++;
              if (MenNum > AmountMensaje)
                {                                                     //Subir en la seleccion de la carpeta
                  MenNum = 2;
                }
              delay(275);
            }
          else if(analogRead(A15) < AbajoMax && analogRead(A15) > AbajoMin || digitalRead(33) == 0)
            {
              Serial.println(analogRead(A15));
              Control = 1;
              MenNum--;
              if (MenNum < 2)
                {                                                   //Bajar en la seleccion de la carpeta
                  MenNum = AmountMensaje;
                }
              delay(275);
            }
         else if(analogRead(A15) < LlamarMax && analogRead(A15) > LlamarMin || digitalRead(41) == 0)
            {
              Serial.println(analogRead(A15));
              Llamada();
              Control = 1;
              delay(275);
            }
          
           else if(analogRead(A15) < OkMax && analogRead(A15) > OkMin || digitalRead(35) == 0)
            {
              Serial.println(analogRead(A15));
              if (MenNum == 7)
                {
                  MensajePersonalizado();
                }
              else if (MenNum == 8)
                {
                  NumeroPersonalizado();
                }
              Control = 1;
              while(analogRead(A15) > AtrasMax && digitalRead(37) == 1)
              {
                Serial.println(analogRead(A15));
                Control2 = 0;
                myGLCD.setColor(0,0,153);
                myGLCD.fillRect(0,120,480,190);
                myGLCD.setColor(153,255,255);
                myGLCD.setBackColor(0,0,153);
                myGLCD.print(MesNew[MenNum-2][MenNum1],LEFT,140);
                delay(275);
                //--------------------------------------------------------------------------------
                while(Control2 == 0)
                  {
                    if(analogRead(A15) < ArribaMax&& analogRead(A15) > ArribaMin || digitalRead(31) == 0)
                      {
                        Serial.println(analogRead(A15));
                        Control2 = 1;
                        MenNum1++;
                        if (MenNum1 > AmountMesNew[MenNum-2])
                          {
                            MenNum1 = 0;                        //Subir en la seleccion del mensaje
                          }
                        delay(275);
                      }
          //--------------------------------------------------------------------------------
                     else if(analogRead(A15) < AbajoMax && analogRead(A15) > AbajoMin || digitalRead(33) == 0)
                      {
                        Serial.println(analogRead(A15));
                        Control2 = 1;
                        MenNum1--;
                        if (MenNum1 < 0)
                          {                                   //Bajar en la seleccion del mensaje
                            MenNum1 = AmountMesNew[MenNum-2];
                          }
                        delay(275);
                      }
                    else if(analogRead(A15) < AtrasMax || digitalRead(37) == 0)
                      {
                        Serial.println(analogRead(A15));
                        Control2 = 1;                       //Volver a la seleccion de la carpeta
                        MenNum1 = 0;
                        delay(275);
                      }
                    else if(analogRead(A15) < LlamarMax && analogRead(A15) > LlamarMin || digitalRead(41) == 0)
                      {
                        Serial.println(analogRead(A15));
                        Llamada();
                        Control2 = 1;
                        delay(275);
                      }
                    else if(analogRead(A15) < OkMax && analogRead(A15) > OkMin)
                      {
                        Serial.println(analogRead(A15));
                        switch(MenNum)
                          {
                            case 2://Afirmaciones
                            Musica = MenNum1+1;
                            break;
                            case 3://Negaciones
                            Musica = MenNum1+ArrayNumber[0]+1;
                            break;
                            case 4://Estado
                            Musica = MenNum1+ArrayNumber[1]+1;
                            break;
                            case 5://Numeros
                            Musica = MenNum1+ArrayNumber[2]+1;
                            break;
                            case 6://Maldiciones
                            Musica = MenNum1+ArrayNumber[3]+1;
                            break;
                          }
                        Reproducir();
                        delay(275);
                      }
            }
  
          }
        }
      }
    }
}

void NumeroPersonalizado()
{
    Control3 = 30;
    CaracterI = 0;
    CaracterF = 18;
    Control8 = 0;
    myGLCD.setColor(0,0,153);
    myGLCD.fillRect(0,70,480,320);
    myGLCD.setTextSize(3);
    while(analogRead(A15)> AtrasMax && digitalRead(37) == 1)
      {
        //---------------------------------------------------------------------------------------------------------------
        if(analogRead(A15) < ArribaMax && analogRead(A15) > ArribaMin || digitalRead(31) == 0)
          {
            Control3++;
            if(Control3 > AlfabetoLong)
              {
                Control3 = AlfabetoLong - 9;
              }
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(CaracterI,118,CaracterF,158);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Alfabeto[Control3],CaracterF - 18,123);
            delay(275);
          }
//---------------------------------------------------------------------------------------------------------------
        else if(analogRead(A15) < AbajoMax && analogRead(A15) > AbajoMin || digitalRead(33) == 0)
          {
            Control3--;
            if(Control3 < AlfabetoLong - 9)
              {
                Control3 = AlfabetoLong;
              }
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(CaracterI,118,CaracterF,158);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Alfabeto[Control3],CaracterF - 18,123);
            delay(275);
          }
//---------------------------------------------------------------------------------------------------------------
        else if(analogRead(A15) < OkMax && analogRead(A15) > OkMin || digitalRead(35) == 0)
          {
            CaracterI = CaracterF;
            CaracterF = CaracterF + 18;
            if(CaracterF >= 204)
              {
                myGLCD.setColor(0,0,153);
                myGLCD.fillRect(0,70,480,200);
                CaracterI = 0;
                CaracterF = 18;
                Control8 = 0;
              }
            NewNum[Control8] = Alfabeto[Control3][0];
            Control8++;
            delay(275);//175
          }
        //---------------------------------------------------------------------------------------------------------------
         else if(analogRead(A15) < BorrarMax && analogRead(A15) > BorrarMin || digitalRead(39) == 0)
          {
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(CaracterF - 36,118,CaracterF,158);
            CaracterI = CaracterI - 18;
            CaracterF = CaracterF - 18;
            Control8--;
            if(CaracterI < 0)
              {
                Control8 = 0;
                CaracterI = 0;
                CaracterF = 18;
              }
            delay(275);//175
          }
      }
  }
void MensajePersonalizado()
  { 
    CaracterF = 18;
    Control8 = 0;
    myGLCD.setColor(0,0,153);
    myGLCD.fillRect(0,70,480,320);
    myGLCD.setTextSize(3);
    while(analogRead(A15)> AtrasMax && digitalRead(37) == 1)
      {
        //---------------------------------------------------------------------------------------------------------------
        if(analogRead(A15) < ArribaMax && analogRead(A15) > ArribaMin || digitalRead(31) == 0)
          {
            Control3++;
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(CaracterI,118,CaracterF,158);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Alfabeto[Control3],CaracterF - 18,123);
            if(Control3 > AlfabetoLong)
              {
                Control3 = 0;
              }
            delay(275);
          }
//---------------------------------------------------------------------------------------------------------------
        else if(analogRead(A15) < AbajoMax && analogRead(A15) > AbajoMin || digitalRead(33) == 0)
          {
            Control3--;
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(CaracterI,118,CaracterF,158);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Alfabeto[Control3],CaracterF - 18,123);
            if(Control3 < 0)
              {
                Control3 = AlfabetoLong;
              }
            delay(275);
          }
//---------------------------------------------------------------------------------------------------------------
        else if(analogRead(A15) < OkMax && analogRead(A15) > OkMin || digitalRead(35) == 0)
          {
            CaracterI = CaracterF;
            CaracterF = CaracterF + 18;
            if(CaracterF >= 410)
              {
                myGLCD.setColor(0,0,153);
                myGLCD.fillRect(0,70,480,200);
                CaracterI = 0;
                CaracterF = 18;
              }
            delay(275);//175
          }
        //---------------------------------------------------------------------------------------------------------------
         else if(analogRead(A15) < BorrarMax && analogRead(A15) > BorrarMin || digitalRead(39) == 0)
          {
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(CaracterF - 36,118,CaracterF,158);
            CaracterI = CaracterI - 18;
            CaracterF = CaracterF - 18;
            if(CaracterI < 0)
              {
                CaracterI = 0;
                CaracterF = 18;
              }
            delay(275);//175
          }
      }
  }
void Llamada()
{
  if (Control9 == 0)
  {
    delay(200);
  int Estatus = 0;//fona.getCallStatus();
  if(Estatus == 0)
    {
      myGLCD.setColor(0,0,153);
      myGLCD.fillRect(0,50,480,240);
      myGLCD.setColor(0,255,0);
      myGLCD.setBackColor(0,0,153);
      myGLCD.print(F("Seleccione el numero  que desea marcar"),LEFT,50);
      while(Control7 == 0)
      {
        Control5 = 0;
        switch (Control6)
          {
            case 0:
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,100,480,240);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Num0,LEFT,150);
            break;
            case 1:
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,100,480,240);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Num1,LEFT,150);
            break;
            case 2:
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,100,480,240);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Num2,LEFT,150);
            break;
            case 3:
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,100,480,240);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(Num3,LEFT,150);
            break;
            case 4:
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,100,480,240);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(NewNum,LEFT,150);
            break;
          }
        while(Control5 == 0)
          {
            if(analogRead(A15) < ArribaMax && analogRead(A15) > ArribaMin || digitalRead(31) == 0)
              {
                Control5 = 1;
                Control6++;
                if(Control6 > 4)
                  {
                    Control6 = 0;
                  }
                delay(275);
              }
            else if(analogRead(A15) < AbajoMax && analogRead(A15) > AbajoMin || digitalRead(33) == 0)
              {
                Control5 = 1;
                Control6--;
                if(Control6 < 0)
                  {
                    Control6 = 4;
                  }
                delay(275);
              }
            else if(analogRead(A15)< AtrasMax || digitalRead(37) == 0)
              {
                Control5 = 1;
                Control7 = 1;
                delay(275);
                myGLCD.setColor(0,0,153);
                myGLCD.fillRect(0,50,480,320);
              }
            else if(analogRead(A15) < OkMax && analogRead(A15) > OkMin || digitalRead(35) == 0)
              {
                Control5 = 1;
                Control7 = 1;
              }
          }
      }
      if(Control7 == 1 && analogRead(A15) < OkMax && analogRead(A15) > OkMin || digitalRead(35) == 0)
        {
          switch (Control6)
                {
                  case 0:
                  fona.callPhone(Num0);
                  break;
                  case 1:
                  fona.callPhone(Num1);
                  break;
                  case 2:
                  fona.callPhone(Num2);
                  break;
                  case 3:
                  fona.callPhone(Num3);
                  break;
                  case 4:
                  fona.callPhone(NewNum);
                  break;
                }
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,50,480,240);
            myGLCD.setColor(0,255,0);
            myGLCD.setBackColor(0,0,153);
            myGLCD.print(F("Listo!"),LEFT,100);
            digitalWrite(26,HIGH);
            myGLCD.setColor(0,0,153);
            myGLCD.fillRect(0,50,480,240);
            delay(2000);
        }
    Control7 = 0;
    Control5 = 0;  
  }
  else
  {
    myGLCD.fillRect(0,50,480,240);
    myGLCD.setColor(0,255,0);
    myGLCD.setBackColor(0,0,153);
    myGLCD.print(F("Colgando"),LEFT,100);
    fona.hangUp();
    digitalWrite(26,LOW);
    delay(2000);
  }
  delay(275);
}
else
  {
    myGLCD.setColor(0,0,153);
    myGLCD.fillRect(0,50,480,240);
    myGLCD.setColor(0,255,0);
    myGLCD.setBackColor(0,0,153);
    myGLCD.print(F("Modo sin conexion"),LEFT,100);
    delay(4000);
  }
}


void Reproducir()
{
  mp3_play(Musica);
  delay(50);
}
  

