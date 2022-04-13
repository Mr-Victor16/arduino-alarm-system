#include <Wire.h>
#include <EEPROM.h>

int pinyB = 0;
int pinyD = 0;
bool alarm = 0;
int status = 1;

void setup()
{
  pinMode(6,INPUT);
  digitalWrite(6, HIGH);
  pinMode(13,INPUT);
  pinMode(2,INPUT);
  
  Wire.begin(2);
  Wire.onRequest(request);
  Wire.onReceive(receive);
  
  if(EEPROModczyt(0) == 0){
    pinyB = PINB;
    pinyD = PIND;
    
    EEPROMwpisz(0,1);
    EEPROMwpisz(2,alarm);
    EEPROMwpisz(4,pinyB);
    EEPROMwpisz(6,pinyD);
    EEPROMwpisz(8,status);
  }
  else{
    alarm = EEPROModczyt(2);
    pinyB = EEPROModczyt(4);
    pinyD = EEPROModczyt(6);
    status = EEPROModczyt(8);
  }
}

void request(){
  Wire.write(alarm);
  alarm = 0;
  EEPROMupdate(2,alarm);
}

void receive(int howMany){
  int x = Wire.read();
  
  if(x==0 || x==1){
    status=x; 
    EEPROMupdate(8,x);
  }
  if(x==0){
    alarm=0;
  }else{
    pinyB = PINB;
    pinyD = PIND;
    
    EEPROMupdate(4,pinyB);
    EEPROMupdate(6,pinyD);
  }
  
}

void EEPROMwpisz(int pozycja, int wartosc){
  byte olderPart = wartosc >> 8;
  byte youngerPart = wartosc;
  
  EEPROM.write(pozycja,olderPart);
  EEPROM.write(pozycja+1,youngerPart);
}

void EEPROMupdate(int pozycja, int wartosc){
  byte olderPart = wartosc >> 8;
  byte youngerPart = wartosc;
  
  EEPROM.update(pozycja,olderPart);
  EEPROM.update(pozycja+1,youngerPart);
}

int EEPROModczyt(int pozycja){
  byte olderPart;
  byte youngerPart;
  
  olderPart = EEPROM.read(pozycja);
  youngerPart = EEPROM.read(pozycja+1);
  
  int wartosc = ((olderPart << 8) + youngerPart);
  return wartosc;
}

void loop()
{
  if(status == 1){
    if((PIND != pinyD) || (PINB != pinyB)){
    	delay(20);
      	alarm = 1;
      	EEPROMupdate(2,alarm); 
      
      	pinyB = PINB;
      	EEPROMupdate(4,pinyB);
      	pinyD = PIND;
      	EEPROMupdate(6,pinyD);
    }
  }
}