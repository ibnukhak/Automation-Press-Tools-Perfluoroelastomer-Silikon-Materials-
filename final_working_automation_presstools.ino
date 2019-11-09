// copyright intership 2018 UNY EE
// modify 18/10/2018 - intership 2018 UMS EE

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <MAX6675_Thermocouple.h>
#include <Adafruit_MCP4725.h>
#define SCK_PIN 8                 //thermocouple, pin SCK ke pin 8 arduino 
#define CS_PIN  9                 //thermocouple, pin CS ke pin 9 arduino
#define SO_PIN  10                //thermocouple, pin SO ke pin 10 arduino

#define READING_NUMBER 10
#define DELAY_TIME  100
#define AUTOMATIC_MODE HIGH
#define MANUAL_MODE LOW
//#define DEBOUNCE_TIME 200
MAX6675_Thermocouple* thermocouple = NULL;

Adafruit_MCP4725 dac;

LiquidCrystal_I2C lcd(0x3f, 20, 4);

//user
const int buttonStart = A0;       //button start, ke pin A0 arduino
const int buttonSelector = A1;    //button selector, ke pin A1 arduino
const int buttonForward = 12;     //button forward, ke pin 12 arduino 
const int buttonBackward = 13; //2     //button backward, ke pin 2 arduino

//yang mau di kontrol
const int relayValveForward = 7;    //Silinder Down  //ralay valve untuk forward ke pin 7 arduino 
const int relayValveBackward = 6;   //Silinder Up //relay Valve untuk backward ke pin 6 arduino

const int Heater1 = 5; //5             // SSR heater //heater 1 di pin 5 arduino
const int Heater2 = 4; //4             // SSR heater //heater 2 di pin 4 arduino

double volatile setPoint = 0.0;

//int automaticStage = 0;
//bool setTimer = false;

int sensorITV = 0;//ITV PROGRAM
int statusStart = 0;// AUTO PROGRAM
int statusForward = 0;// MANUAL Maju PROGRAM
int statusBackward = 0;// MANUAL Mundur PROGRAM
int statusSelector = 0;//SELECTOR PROGRAMee1

void controlTemp(void)
{
  double temp = readTemp();
  if (temp < setPoint) {
    //nyalain heater
    //Serial.println("ON");
    digitalWrite(Heater1, HIGH);
    digitalWrite(Heater2, HIGH);
  } else {
    //matiin heater
    //Serial.println("OFF");
    digitalWrite(Heater1, LOW);
    digitalWrite(Heater2, LOW);
  }
}

void setup ()
{

  Serial.begin(9600);
  thermocouple = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN, READING_NUMBER, DELAY_TIME  );
  dac.begin(0x62);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" WAIT TEMP 135C ");


  Timer1.initialize(1000000);
  Timer1.attachInterrupt(controlTemp); //
  pinMode(buttonStart, INPUT_PULLUP);    // declare buttonStart as input + activate internal pull-up resistor
  pinMode(buttonForward, INPUT_PULLUP);  // declare button Manual as input + activate internal pull-up resistor
  pinMode(buttonBackward, INPUT_PULLUP);  // declare button Manual as input + activate internal pull-up resistor
  pinMode(buttonSelector, INPUT_PULLUP); // declare button Manual as input + activate internal pull-up resistor
  pinMode(relayValveForward, OUTPUT);
  pinMode(relayValveBackward, OUTPUT);
  pinMode(Heater1, OUTPUT);
  pinMode(Heater2, OUTPUT);
  
 setTemp(135.0); // atur suhu awal;
 waitTemp(135.0);//tunggu temperatue baru bisa jalankan program auto
  lcd.clear();
}

void loop ()
{
  const double temp =  thermocouple->readCelsius();
  //setTemp(135.0); //135.0 //I
  delay(500);
  //waitTemp(135.0); //I
  
  //Serial.println(String(temp));
  if ((statusSelector == AUTOMATIC_MODE)) {

    lcd.setCursor(0, 0);
    lcd.print ("Auto Mode       ");
    lcd.setCursor(7, 1);
    lcd.print("Wait 135C");
    lcd.setCursor(0, 1);
    lcd.print(String(temp));
    delay(200);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print ("Manual Mode     ");
    lcd.setCursor(7, 1);
    lcd.print("Wait 135C");
    lcd.setCursor(0, 1);
    lcd.print(String(temp));
    delay(200);

  }

  statusSelector = digitalRead(buttonSelector);
  //digitalWrite(fan, LOW);
  
  if ((statusSelector == AUTOMATIC_MODE)   )
  { Serial.println("AUTO MODE, waiting for button press");
    if (startON()) {
      Serial.println("AUTO MODE, waiting for button press");
      // lcd.setCursor(0, 0)      ;
      //digitalWrite(, LOW);
      //program untuk otomatis
      Serial.println("Doing seq 0");
        const double temp =  thermocouple->readCelsius();
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Celcius");
        lcd.setCursor(0, 0);
        lcd.print(setPoint);
        lcd.setCursor(0, 1);
        lcd.print("Wait 10 min");
        automaticStage(0);
      Serial.println("Doing seq 1");
        //const double temp =  thermocouple->readCelsius();
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Celcius");
        lcd.setCursor(0, 0);
        lcd.print(setPoint);
        lcd.setCursor(7, 1);
        lcd.print("PSI");
        automaticStage(1);

      Serial.println("Doing seq 2");
        lcd.setCursor(7, 0);
        lcd.print("Celcius");
        lcd.setCursor(0, 0);
        lcd.print(setPoint);
        lcd.setCursor(7, 1);
        lcd.print("PSI");
        automaticStage(2);

      Serial.println("Doing seq 3");
        lcd.setCursor(7, 0);
        lcd.print("Celcius");
        lcd.setCursor(0, 0);
        lcd.print("190.00");
        lcd.setCursor(7, 1);
        lcd.print("PSI");
        automaticStage(3);

      Serial.println("Doing seq 4");
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Celcius");
        lcd.setCursor(0, 0);
        lcd.print(setPoint);
        lcd.setCursor(7, 1);
        lcd.print("PSI");
        automaticStage(4);

      Serial.println("Doing seq 5");
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Celcius");
        lcd.setCursor(0, 0);
        lcd.print("30.00");
        lcd.setCursor(7, 1);
        lcd.print("PSI");
        automaticStage(5);
        
        Serial.println("Doing seq : Done");
        digitalWrite(relayValveBackward, HIGH); //low
        digitalWrite(relayValveForward, LOW); //high

        lcd.clear();
        lcd.print("FINISH");
        lcd.setCursor(5, 0);
        delayMinute(10);
    }

  }

  else
  {
    //manual selector mode
   
  //    digitalWrite(relayValveForward, HIGH);  //I
  //  digitalWrite(relayValveBackward, HIGH); //I
    
    if (!digitalRead(buttonForward))  {
      digitalWrite(relayValveForward, LOW); //L
      digitalWrite(relayValveBackward, HIGH); //H
      setPressure(15.0); //15
    }

    if (!digitalRead(buttonBackward))  {
      digitalWrite(relayValveBackward, LOW); //L
      digitalWrite(relayValveForward, HIGH); //H
    }

  }

}


bool startON() {
  return !digitalRead(buttonStart);
}

void automaticStage(int seqNum) {
  double currentTemp = 135.0;

  switch (seqNum) {
    case 0:
    //No Pressure
      setTemp(135.0);
      delayMinute(10); //10
      break;
    case 1:
      setTemp(135.0);
      setPressure(5.0);
      digitalWrite(relayValveForward, HIGH); //low
      digitalWrite(relayValveBackward, LOW); //high
      delayMinute(2); //2

      break;

    case 2:
      setTemp(135.0);
      setPressure(15.0);
      delayMinute(8); //8
      break;

    case 3:
      setPressure(60.0);
      setTemp(190.0);
      delayMinute(10); //10
      break;
    case 4:
      setTemp(190.0);
      setPressure(60.0);
      delayMinute(10); //10
      break;

    case 5:
      int fan  = 3;
      pinMode(fan, OUTPUT);
      digitalWrite(fan, LOW);
      setTemp(30.0);
      setPressure(60.0);
      delayMinute(25); //25
      digitalWrite(fan, HIGH);
      break;


  }
}


//---Function--

void waitTemp(double temp)
{
  Serial.println("Wait Temp: start");
  while (readTemp() < temp)
  {
    delay(700);
  }
  Serial.println("Wait Temp: end");
}

void delayMinute(int minutes) {
  int p = minutes * 60;
  for (int i = 0; i < p; i++) {
    //lcd.setCursor(5, 1);
    delay(850);
  }
}

void setTemp(double temp) {
  setPoint = temp;
}

double readTemp()
{
  const double temp =  thermocouple-> readCelsius();
  Serial.println(String(temp));
  return temp;
  //lcd.setCursor(5,1);
  //lcd.print(setPoint);
  //delay(300);
}

void setPressure(float pressure) {
  
  int val = int(map(pressure, 0.0, 60.0, 0, 4095));
  dac.setVoltage(val, false);
  lcd.setCursor(0, 1);
  lcd.print(pressure);
}
