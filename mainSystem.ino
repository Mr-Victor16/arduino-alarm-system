#include <Adafruit_NeoPixel.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {14, 15, 16, 17};
byte colPins[COLS] = {13, 12, 11, 10}; 

int stanAlarmu = 1;
int iloscUrzadzen=0;
int adresy[128];
int iloscBlednych=0;

String tPIN = "";
int wpisywanie=0;
int tryb=0;

char keys[ROWS][COLS] = { 
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad klawiatura = Keypad (makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(9,8,5,4,3,2);
Adafruit_NeoPixel diody = Adafruit_NeoPixel(6, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  Wire.begin();
  lcd.begin(16,2);
  diody.begin();
  pinMode(7, OUTPUT);
  
  komunikat("Uruchamianie","alarmu",20);
  if(konfiguracja()==true){
   komunikat("Sukces","",200);
   konfiguracja(); 
    
    if(stanAlarmu==0){
      komunikat("Witaj","",20);
    }
  }
  else{
    komunikat("Blad","uruchamiania",200);
  }
}

int odczytIlosciUrzadzen(){
  int ilosc=0;
    
  for(int i=0; i<127; i++){
    Wire.beginTransmission(i);
    int error = Wire.endTransmission();
      
    if(error==0){
      ilosc++;
      adresy[ilosc]=i;
    }
  }
  
  return ilosc;
}

bool konfiguracja(){
  bool status = true;
  
  if(EEPROModczyt(8) == 0){
    iloscUrzadzen = odczytIlosciUrzadzen();
    if(iloscUrzadzen == 1){
      status = false;
      stanAlarmu=4;
    }
    
    EEPROMwpisz(0, 4321);
    EEPROMwpisz(2, 1234);
    EEPROMwpisz(4, 1);
    EEPROMwpisz(8, iloscUrzadzen);
  }
  else{
    stanAlarmu = EEPROModczyt(4);
    iloscUrzadzen = EEPROModczyt(8);
    
    if(odczytIlosciUrzadzen() == iloscUrzadzen){
      int ile = 0;
    	for(int i=0; i<=127; i++){ 
      		Wire.beginTransmission(adresy[i]);
            if(stanAlarmu==0 || stanAlarmu==4){
            	Wire.write(0);
            }
            else{
              Wire.write(1);
            }

      		int error = Wire.endTransmission();
            if(error==0){
              ile++;
              
                if(stanAlarmu==0){
                  diodaLED(0,255,0,i,0); 
                }
                else if(stanAlarmu==1){
                  diodaLED(0,0,255,i,0); 
                }
                else{
                  diodaLED(255,0,0,i,0); 
                }
            }
    	}
  
      if(ile==0 || iloscUrzadzen != ile){
        status=false;
        stanAlarmu=4;
      }
    }
    else{
      status=false;
      stanAlarmu=4;
    }
  }
  
  return status;
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

bool sprawdzPIN(int pin, bool typUzytkownika){
  int sprawdzany;
  
  if(typUzytkownika == 0){
    sprawdzany = EEPROModczyt(0);
  }
  else{
    sprawdzany = EEPROModczyt(2);
  }
  
  if(pin == sprawdzany){
    return true;
  }
  else{
    return false;
  }   
}

void zmienPIN(int pin, bool typUzytkownika){
  if(typUzytkownika == 0){
    EEPROMupdate(0,pin);
  }
  else{
    EEPROMupdate(2,pin);
  }
  
  lcd.clear();
  lcd.print("Zmieniono PIN");
  delay(200);
  lcd.clear();
}

void loop()
{
  char key;
  
  switch(stanAlarmu){
    case 0: //alarm rozbrojony
    {
	  key = klawiatura.getKey();
    
      if(key == 'A'){ //uzbrajanie alarmu
        komunikat("Uzbrajanie","alarmu",200);
        
        int wyslane=0;
        for(int i=0; i<=iloscUrzadzen; i++){
    		Wire.beginTransmission(adresy[i]);
    		Wire.write(1);
        
        	int error = Wire.endTransmission();
          	
            if(error==0){
              diodaLED(0,0,255,i,0);
              wyslane++;
            }
            else{
              diodaLED(255,255,0,i,0);           
            }
      	}
        
        if(wyslane>=1 && iloscUrzadzen==wyslane){
          stanAlarmu = 1;
          EEPROMupdate(4,1);
          
          komunikat("Alarm uzbrojony","",200);
        }
        else{
          for(int i=0; i<=iloscUrzadzen; i++){
    		Wire.beginTransmission(adresy[i]);
    		Wire.write(0);
        	Wire.endTransmission();
      	  }
          
          komunikat("Blad","",200);
        }
      }
      else if(key == 'B'){  //zmiana pinu uzytkownika
        komunikat("Zmiana pinu","uzytkownika",200);
        lcd.clear();
        lcd.print("Wprowadz PIN: ");
        lcd.setCursor(11,1);
        
        tPIN="";
        stanAlarmu=5;
        wpisywanie=1;
        tryb=2;
      }
      else if(key == 'C'){  //zmiana pinu admina
       	komunikat("Zmiana pinu","admina",200);
        lcd.clear();
       	lcd.print("Wprowadz PIN: ");
        lcd.setCursor(11,1);
        
        tPIN="";
        stanAlarmu=5;
        wpisywanie=1;
        tryb=3;
      }
      
      else if(key == 'D'){ //tryb konfiguracji
        komunikat("Tryb","Konfiguracji",200);
        lcd.clear();
        lcd.print("Wprowadz PIN: ");
        lcd.setCursor(11,1);
        
        tPIN="";
        stanAlarmu=5;
        wpisywanie=1;
        tryb=4;
      }
    }
    break;
    
    case 1: //monitorowanie
    {
      int licznik=0;
      
      for(int i=1; i<=iloscUrzadzen; i++){
    	Wire.beginTransmission(adresy[i]);
    	Wire.requestFrom(adresy[i], 1);
        bool odpowiedz;
        
        while(Wire.available()){
     		odpowiedz = Wire.read();
    	}
        
        int error = Wire.endTransmission();
        
        if(error==0){
          if(odpowiedz==1){
            stanAlarmu = 3;
            EEPROMupdate(4,3);
            diodaLED(255,0,0,i,0); 
            delay(200);
          }
          licznik++;
        }
        else{
          diodaLED(255,255,0,i,0); 
        }
      }
      
      if(licznik==0 || licznik!=iloscUrzadzen){
       stanAlarmu=4; 
      }
      
      char wcisniety = klawiatura.getKey();
      if(wcisniety){
        stanAlarmu=2; 
        lcd.clear();
      	lcd.print("Rozbrajanie:");
        lcd.setCursor(0,1);
        tPIN="";
      }
    }
    break;
    
    case 2: // rozbrajanie
    { 
        if(tPIN.length()<4){
          char wcisniety = klawiatura.getKey();
          if(wcisniety){
              switch(wcisniety){
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                {
                  tPIN += wcisniety;
                  lcd.print(wcisniety);
                }
              }
          }
        }

       else{
          int pin = tPIN.toInt();
          tPIN="";

        if(sprawdzPIN(pin, 0) || sprawdzPIN(pin, 1)){
          lcd.clear();
          komunikat("PIN prawidlowy","",200);
          lcd.clear();
          EEPROMupdate(4,0);
          stanAlarmu = 0;

          for(int i=0; i<=iloscUrzadzen; i++){
              Wire.beginTransmission(adresy[i]);
              Wire.write(0);
              Wire.endTransmission();
          }

          pasekLED(0,255,0,50);
          komunikat("Witaj","",200);
          break;
        }
        else{
          komunikat("Bledny PIN","",200);
          lcd.clear();
          stanAlarmu = 1;
          iloscBlednych++;

          if(iloscBlednych==3){
              stanAlarmu=3; 
              EEPROMupdate(4,3);
              iloscBlednych=0;
          }
        }
       }
    }
    break;
    
    case 3: // alarm
    {
      komunikat("Alarm","",200);
      pasekLED(255,0,0,10);
      EEPROMupdate(4,3);
      tone(7, 1500);
      delay(50);
      noTone(7);
      stanAlarmu=1;
    }
    break;
    
    case 4: //ponowna proba konfiguracji w przypadku błędów
    {
      komunikat("Wykryto blad w","dzialaniu alarmu",300);
      komunikat("Ponowna proba","konfiguracji",400);
      
      int odczyt = odczytIlosciUrzadzen();
      if(odczyt > 1){
        komunikat("Sukces","",300);
        iloscUrzadzen=odczyt;
        EEPROMupdate(8, odczyt);
        stanAlarmu=EEPROModczyt(4);
        pasekLED(0,0,255,10);
      }
      else{
        komunikat("Blad","",300);
        pasekLED(255,0,0,10);
        stanAlarmu=4;
        delay(500);
      }
    }
    break;
    
    case 5:
    {
      if(tPIN.length()<4){
        char wcisniety = klawiatura.getKey();
        if(wcisniety){
          switch(wcisniety){
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
              tPIN += wcisniety;
              lcd.print(wcisniety);
            }
           }
        }
      }
      else{
        if(wpisywanie==1){
          if(sprawdzPIN(tPIN.toInt(), 1) == true){
            lcd.clear();
            lcd.print("PIN prawidlowy");
            tPIN="";
            delay(200);
            lcd.clear();
            wpisywanie=0;
            
            if(tryb==2 || tryb==3){
              lcd.clear();
          	  lcd.print("Nowy PIN: ");
            }
            else if(tryb==4){
              int ile = odczytIlosciUrzadzen();
              if(ile==1){
                 lcd.clear();
                 lcd.print("Blad");
              }
              else{
                EEPROMupdate(8, ile);
                iloscUrzadzen=ile;
                lcd.clear();
                pasekLED(0,255,0,10);
                lcd.print("Sukces");
              }
              
              delay(200);
              lcd.clear();
              stanAlarmu=0;
          	  tryb=0;
            }
          }
          else{
            lcd.clear();
            lcd.print("Bledny PIN");
            tPIN="";
            delay(200);
            lcd.clear();
            stanAlarmu=0;
          }
        }
        else if(tryb==2){
          zmienPIN(tPIN.toInt(), 0);
          stanAlarmu=0;
          tryb=0;
        }
        else if(tryb==3){
          zmienPIN(tPIN.toInt(), 1);
          stanAlarmu=0;
          tryb=0;
        }
      }
    }
    break;
  }
}

void komunikat(String str1, String str2, int czas){
  lcd.clear();
  lcd.print(str1);
  
  if((str1.length()+str2.length()) > 15){
    lcd.setCursor(0,1);
  } 
  else{
   lcd.print(" ");
  }
  
  lcd.print(str2);
  delay(czas);
}

void pasekLED(int r, int g, int b, int czas){
  diody.clear();
  diody.show();
  delay(20);
  for(int i=0; i<iloscUrzadzen; i++){
     diody.setPixelColor(i, diody.Color(r,g,b));
     delay(czas);
   }
  diody.show();
}

void diodaLED(int r, int g, int b, int numer, int tryb){
  if(tryb==1 && numer==iloscUrzadzen){
    diody.setPixelColor(numer, diody.Color(r,g,b));
    diody.show();
    
    delay(50);
    diody.clear();
  }
  else{
    diody.setPixelColor(numer-1, diody.Color(r,g,b));
    diody.show();
  }
}