#include <LiquidCrystal.h>
#include <EEPROM.h>


//definizione dei pin
#define pinSelect 8               //ingresso digitale del tasto select (impulso negativo alla pressione)
#define pinRight  9               //ingresso digitale del tasto right (impulso negativo alla pressione)
#define pinLeft   10              //ingresso digitale del tasto left (impulso negativo alla pressione)
#define resIn     0               //ingresso analogico per la fotoresistenza
#define relay1    11              //uscita digitale del relè 1
#define relay2    12              //uscita digitale del relè 2
#define refresh   100             //tempo di attesa per nuovo campionamento
#define ResetSec  86400           //secondi dopo la quale il microcontrollore si riavvia automaticamente
LiquidCrystal lcd(7,6,5,4,3,2);   //definizione dei piedini del display

//definizione debug mode
#define DebugMode false    //abilita la modalità di debug (esclude tutto l'hardware input esterno e prende in considerazione gli input solamente la porta seriale)
#define SerialOut false    //abilita o disabilita l'output anche su porta seriale (da abilitare se si vuole eseguire il debug esclusivamente su porta seriale)
#define baudrate  9600    //velocità della porta seriale (utilizzata per il debug)

//definizione delle configurazioni
#define ConfigDelay     200   //ritardo nella fase di configurazione
#define eepromMode      0     //locazione  contenente la modalità di configurazione
#define eepromMinVal    1     //locazione contenente il primo valore minimo (comprende 2 bytes)
#define eepromMinVal2   9     //locazione contenente il secondo valore minimo (comprende 2 bytes)
#define eepromDelay1    3     //locazione contenente il ritardo dello spegnimento del primo relè(comprende 2 bytes)
#define eepromDelay2    5     //locazione contenente il ritardo di accensione del primo relè (comprende 2 bytes)
#define eepromDelay3    7     //locazione contenente il ritardo dell'allarme (comprende 2 bytes)
#define eepromDelay4    11    //locazione contenente il ritardo dello spegnimento del primo relè(comprende 2 bytes)
#define eepromDelay5    13    //locazione contenente il ritardo di accensione del primo relè (comprende 2 bytes)


void printLcd(char* output, unsigned int row, unsigned int col);
void printLcd(int output, unsigned int row, unsigned int col);
void printLcdN(char* output);
void printLcdN(int output);
unsigned int buttonIn();
void modeSetup();
void reset();
void dataReload();
unsigned int photoIn();
void(* Reset)(void) = 0;

unsigned int mode;
word delay1;
word delay2;
word delay3;
word delay4;
word delay5;
word value1;
word value2;
unsigned int valIn=0;
bool dis=false;

void setup() {
  Serial.begin(baudrate);             //definizione pins
  lcd.begin(16, 2);
  pinMode(pinSelect, INPUT);
  pinMode(pinRight, INPUT);
  pinMode(pinLeft, INPUT);
  pinMode(resIn, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  if (DebugMode==true) {
    modeSetup();
  }
  dataReload();
  switch(mode) {
    case 0:
    loop1();
    break;
    
    case 1:
    loop2();
    break;
    
    case 2:
    loop3();
    break;
    
    case 3:
    loop4();
    break;
    
    default:
    modeSetup();
  }
}

void loop2() {
  bool dis=true;
  int i=0;
  unsigned long mil=0;
  while(true) {
    if (millis()>=(ResetSec*1000) && DebugMode==false) {
      Reset();
    }
    if (DebugMode==false) {
      switch(buttonIn()) {
        case 2:
        modeSetup();
        break;

        case 4:
        reset();
        break;

        case 5:
        relayTest();
        break;
      }
    }
    
    if (dis==false) {
      digitalWrite(relay1, true);
      digitalWrite(relay2, true);
    }
    else {
      digitalWrite(relay1,false);
      digitalWrite(relay2,false);
    }
    valIn=photoIn();
    printLcd(" --> ",0,0);
    printLcd(valIn,6,0);
    if (dis==true) {
      if (valIn>value1) {
        i=delay2;
        while(i>0) {
          lcd.clear();
          valIn=photoIn();
          printLcd(" --> ",0,0);
          printLcd(valIn,6,0);
          printLcd("allaccio in ",0,1);
          printLcd(i,13,1);

          if (valIn<value1) {
            break;
          }
          if (mil<(millis()*1000)) {
            i--;
            mil=millis();
          }
        }
        if (i==0) {
          dis=false;
        }
      }
    }
    else {
      if (valIn<value1) {
        i=delay1;
        while(i>0) {
          lcd.clear();
          valIn=photoIn();
          printLcd(" --> ",0,0);
          printLcd(valIn,6,0);
          printLcd("distacco in ",0,1);
          printLcd(i,13,1);

          if (valIn>value1) {
            break;
          }
          if (mil<(millis()*1000)) {
            i--;
            mil=millis();
          }
          
        }
        if (i==0) {
          dis=true;
        }
      }
    }
  }
}

void loop1() {
  bool dis=false;
  bool dis2=false;
  int i=delay1;
  int j=delay4;
  int i2=delay2;
  int j2=delay5;
  unsigned long mil=0;
  while(true) {
    if (millis()>=(ResetSec*1000) && DebugMode==false) {
      Reset();
    }
    if (DebugMode==false) {
      switch(buttonIn()) {
        case 2:
        modeSetup();
        break;

        case 4:
        reset();
        break;

        case 5:
        relayTest();
        break;
      }
    }
    
    if (dis==false) {
      digitalWrite(relay1, true);
    }
    else {
      digitalWrite(relay1,false);
    }
    if (dis2==false) {
      digitalWrite(relay2, true);
    }
    else {
      digitalWrite(relay2, false);
    }

    valIn=photoIn();
    printLcd(" --> ",0,0);
    printLcd(valIn,6,0);
    if (valIn<value1 && dis==false) {
      printLcd("0ff",0,1);
      printLcd(i,4,1);
      if (mil<(millis()*1000)) {
        i--;
        mil=millis();
      }
      //i2=delay2;
    }
    else {
      i=delay1;
    }
    if (valIn<value2 && dis2==false) {
      printLcd("0ff",9,1);
      printLcd(j,12,1);
      if (mil<(millis()*1000)) {
        j--;
        mil=millis();
      }
      //j2=delay5;
    }
    else {
      j=delay4;
    }
    
   
    if (valIn>value1 && dis==true) {
      printLcd("on ",0,1);
      printLcd(i2,4,1);
      if (mil<(millis()*1000)) {
        i2--;
        mil=millis();
      }
      //i=delay1;
    }
    else {
      i2=delay2;
    }
    if (valIn>value2 && dis2==true) {
      printLcd("on ",9,1);
      printLcd(j2,11,1);
      if (mil<(millis()*1000)) {
        j2--;
        mil=millis();
      }
      //j=delay4;
    }
    else {
      j2=delay5;
    }
  
    if (i<0) {
      dis=true;
    }
    if (i2<0) {
      dis=false;
    }
    if (j<0) {
      dis2=true;
    }
    if (j2<0) {
      dis2=false;
    }
  }
  
}

void loop3() {
  bool dis=true;
  int i=0;
  unsigned long mil=0;
  while(true) {
    if (millis()>=(ResetSec*1000) && DebugMode==false) {
      Reset();
    }
    if (DebugMode==false) {
      switch(buttonIn()) {
        case 2:
        modeSetup();
        break;

        case 4:
        reset();
        break;

        case 5:
        relayTest();
        break;
      }
    }
    if (dis==false) {
      digitalWrite(relay1, true);
      digitalWrite(relay2, false);
    }
    else {
      digitalWrite(relay1,false);
      digitalWrite(relay2,true);
    }
    valIn=photoIn();
    printLcd(" --> ",0,0);
    printLcd(valIn,6,0);
    if (dis==true) {
      if (valIn>value1) {
        i=delay2;
        while(i>0) {
          lcd.clear();
          valIn=photoIn();
          printLcd(" --> ",0,0);
          printLcd(valIn,6,0);
          printLcd("allaccio in ",0,1);
          printLcd(i,13,1);

          if (valIn<value1) {
            break;
          }
          if (mil<(millis()*1000)) {
            i--;
            mil=millis();
          }
          
        }
        if (i==0) {
          dis=false;
        }
      }
    }
    else {
      if (valIn<value1) {
        i=delay1;
        while(i>0) {
          lcd.clear();
          valIn=photoIn();
          printLcd(" --> ",0,0);
          printLcd(valIn,6,0);
          printLcd("distacco in ",0,1);
          printLcd(i,13,1);

          if (valIn>value1) {
            break;
          }
          if (mil<(millis()*1000)) {
            i--;
            mil=millis();
          }
          
        }
        if (i==0) {
          dis=true;
        }
      }
    }
  }
}

void loop4() {
  bool dis=true;
  int i=0;
  unsigned long mil=0;
  while(true) {
    if (millis()>=(ResetSec*1000) && DebugMode==false) {
      Reset();
    }
    if (DebugMode==false) {
      switch(buttonIn()) {
        case 2:
        modeSetup();
        break;

        case 4:
        reset();
        break;

        case 5:
        relayTest();
        break;
      }
    }
    if (dis==false) {
      digitalWrite(relay1, true);
    }
    else {
      digitalWrite(relay1,false);
    }
    valIn=photoIn();
    printLcd(" --> ",0,0);
    printLcd(valIn,6,0);
    if (dis==true) {
      if (valIn>value1) {
        i=delay2;
        while(i>0) {
          lcd.clear();
          valIn=photoIn();
          printLcd(" --> ",0,0);
          printLcd(valIn,6,0);
          printLcd("allaccio in ",0,1);
          printLcd(i,13,1);

          if (valIn<value1) {
            break;
          }
          if (mil<(millis()*1000)) {
            i--;
            mil=millis();
          }
        }
        if (i==0) {
          dis=false;
          digitalWrite(relay2, true);
          for (int i=0;i<delay3;i++) {
          delay(1000);
          }
          digitalWrite(relay2, false);
        }
      }
    }
    else {
      if (valIn<value1) {
        i=delay1;
        while(i>0) {
          lcd.clear();
          valIn=photoIn();
          printLcd(" --> ",0,0);
          printLcd(valIn,6,0);
          printLcd("distacco in ",0,1);
          printLcd(i,13,1);

          if (valIn>value1) {
            break;
          }
          if (mil<(millis()*1000)) {
            i--;
            mil=millis();
          }
        }
        if (i==0) {
          dis=true;
          digitalWrite(relay2, true);
          for (int i=0;i<delay3;i++) {
          delay(1000);
          }
          digitalWrite(relay2, false);
        }
      }
    }
  }
}






//funzioni principali
unsigned int photoIn() {
  if (DebugMode==true) {
    Serial.println();
    Serial.print(":");
    Serial.flush();
    int sum=0;
    int i=1;
    while (Serial.available()==0) {}
    while (Serial.available()>0) {
      sum=sum+(Serial.parseInt()*i);
      i=i*10;
    }
    return sum;
  }
  else {
    return analogRead(resIn);
  }
}

unsigned int buttonIn() {
  int input=-1;
  if (DebugMode==true) {
    Serial.flush();
    while(input==-1) {
      input=Serial.read();
      if (input=='-') {
        return 1;
      }
      if (input=='c') {
        return 2;
      }
      if (input=='+') {
        return 3;
      }
      if (input=='r') {
        return 4;
      }
    }
  }
  else {
    if (digitalRead(pinSelect)==false && digitalRead(pinRight)==false) {
      return 5;
    }
    if (digitalRead(pinSelect)==false && digitalRead(pinLeft)==false) {
      return 4;
    }
    if(digitalRead(pinSelect)==false) {
      return 2;
    }
    if(digitalRead(pinLeft)==false) {
      return 1;
    }
    if(digitalRead(pinRight)==false) {
      return 3;
    }
    return 0;
  }
}

void printLcd(char* output, unsigned int row, unsigned int col) {
  if (SerialOut==true) {
    Serial.println(output);
    lcd.setCursor(row, col);
    lcd.print(output);
    lcd.print(" ");
  }
  else {
    lcd.setCursor(row, col);
    lcd.print(output);
  }
}

void printLcd(int output, unsigned int row, unsigned int col) {
  if (SerialOut==true) {
    Serial.println(output);
    lcd.setCursor(row, col);
    lcd.print(output);
    lcd.print(" ");
  }
  else {
    lcd.setCursor(row, col);
    lcd.print(output);
  }
}

void printLcdN(char* output) {
  if (SerialOut==true) {
    Serial.print(output);
    lcd.print(output);
  }
  else {
    lcd.print(output);
  }
}


void printLcdN(int output) {
  if (SerialOut==true) {
    Serial.print(output);
    lcd.print(output);
  }
  else {
    lcd.print(output);
  }
}

void modeSetup() {
  
  unsigned int input=0;
  word value=0;

  int mode=EEPROM.read(eepromMode);
  delay(ConfigDelay);
  do{
    switch(mode) {
      case 0:
      lcd.clear();
      printLcd("modalità: ",0,0);
      printLcd("indipendente",0,1);
      break;

      case 1:
      lcd.clear();
      printLcd("modalità: ",0,0);
      printLcd("unita",0,1);
      break;
    
      case 2:
      lcd.clear();
      printLcd("modalità: ",0,0);
      printLcd("invertita",0,1);
      break;

      case 3:
      lcd.clear();
      printLcd("modalità: ",0,0);
      printLcd("allarme",0,1);
      break;

      default:
      lcd.clear();
      printLcd("modalità: ",0,0);
      printLcd("non definita",0,1);
      break;
      
    }
    
    input=buttonIn();
    if (input==1) {
      if (mode>0) {
        mode--;
      }
    }
    if (input==3) {
      if (mode<3) {
        mode++;
      }
    }
    delay(ConfigDelay);
  }while(input!=2);
  EEPROM.write(eepromMode,mode);
 
  EEPROM.get(eepromMinVal,value);
  do {
    lcd.clear();
    printLcd("minimo 1",0,0);
    printLcd(value,0,1);
    input=buttonIn();
    if (input==1) {
      if (value>0) {
        value--;
      }
    }
    if (input==3) {
      if (value < 65025) {
        value++;
      }
    }
    
    
  }while (input!=2);
  EEPROM.put(eepromMinVal,value);

  if (mode==0) {
    EEPROM.get(eepromMinVal2,value);
    do {
      lcd.clear();
      printLcd("minimo 2",0,0);
      printLcd(value,0,1);
      input=buttonIn();
      if (input==1) {
        if (value>0) {
          value--;
        }
      }
      if (input==3) {
        if (value < 65025) {
          value++;
        }
      }
    
    delay(ConfigDelay);
    }while (input!=2);
    EEPROM.put(eepromMinVal2,value);
  }
  
  EEPROM.get(eepromDelay1,value);
  do {
    lcd.clear();
    printLcd("ritardo 1 off",0,0);
    printLcd(value,0,1);
    input=buttonIn();
    if (input==1) {
      if (value>0) {
        value--;
      }
    }
    if (input==3) {
      if (value < 998) {
        value++;
      }
    }
    delay(ConfigDelay);
  }while (input!=2);
  EEPROM.put(eepromDelay1,value);
  
  EEPROM.get(eepromDelay2,value);
  do {
    lcd.clear();
    printLcd("ritardo 1 on",0,0);
    printLcd(value,0,1);
    input=buttonIn();
    if (input==1) {
      if (value>0) {
        value--;
      }
    }
    if (input==3) {
      if (value < 998) {
        value++;
      }
    }
    delay(ConfigDelay);
  }while (input!=2);
  EEPROM.put(eepromDelay2,value);

  if (mode==0) {
    EEPROM.get(eepromDelay4,value);
    do {
      lcd.clear();
      printLcd("ritardo 2 off",0,0);
      printLcd(value,0,1);
      input=buttonIn();
      if (input==1) {
        if (value>0) {
          value--;
        }
      }
      if (input==3) {
        if (value < 998) {
          value++;
        }
      }
      delay(ConfigDelay);
    }while (input!=2);
    EEPROM.put(eepromDelay4,value);
    EEPROM.get(eepromDelay5,value);
    do {
      lcd.clear();
      printLcd("ritardo 2 on",0,0);
      printLcd(value,0,1);
      input=buttonIn();
      if (input==1) {
        if (value>0) {
          value--;
        }
      }
      if (input==3) {
        if (value < 998) {
          value++;
        }
      }
      delay(ConfigDelay);
    }while (input!=2);
    EEPROM.put(eepromDelay5,value);
  }

  if (mode==3) {
    EEPROM.get(eepromDelay3,value);
    do {
      lcd.clear();
      printLcd("ritardo allarme",0,0);
      printLcd(value,0,1);
      input=buttonIn();
      if (input==1) {
        if (value>0) {
          value--;
        }
      }
      if (input==3) {
        if (value < 998) {
          value++;
        }
      }
      delay(ConfigDelay);
    }while (input!=2);
    EEPROM.put(eepromDelay3,value);
  }
  
  dataReload();
}

void reset() {
  word n=0;
  EEPROM.write(eepromMode,0);
  EEPROM.put(eepromMinVal,n);
  EEPROM.put(eepromMinVal2,n);
  EEPROM.put(eepromDelay1,n);
  EEPROM.put(eepromDelay2,n);
  EEPROM.put(eepromDelay3,n);
  EEPROM.put(eepromDelay4,n);
  EEPROM.put(eepromDelay5,n);
  lcd.clear();
  printLcd("reset",0,0);
  printLcd("effettuato",0,0);
  delay(3000);
  modeSetup();
}

void dataReload() {
  mode=EEPROM.read(eepromMode);       //caricamento dei dai dall'eprom
  EEPROM.get(eepromDelay1,delay1);
  EEPROM.get(eepromDelay2,delay2);
  EEPROM.get(eepromDelay3,delay3);
  EEPROM.get(eepromDelay4,delay4);
  EEPROM.get(eepromDelay5,delay5);
  EEPROM.get(eepromMinVal,value1);
  EEPROM.get(eepromMinVal2,value2);
  lcd.clear();
}

void relayTest() {
  for(int i=0;i<5;i++) {
    digitalWrite(relay1, false);
    delay(100);
    digitalWrite(relay1, true);
    delay(100);
  }
  for(int i=0;i<5;i++) {
    digitalWrite(relay2, false);
    delay(100);
    digitalWrite(relay2, true);
    delay(100);
  }
}

void loop() {}
