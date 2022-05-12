/*//////////////////////////////////////////////////
//  Project Padvolginator                         //
//Door: Stephan, Skip, Ruben en Tristan           //
//Datum: 04/01/2020                               //
// Stephan strengers: 1786764                     //
// Skip Wijtman: 1787390                          //
// Tristan de gans: 1786830                       //
// Ruben Middelman: 1740398                       //
*///////////////////////////////////////////////////

#include <Servo.h>

  //Prototypes
  void setup();
  void loop();
  int Afstand_Sensor();
  void SensorWaardeLezen();
  void CasePicker(int WelkeCase);
  void CaseFinder();
  void Rechtdoor();
  void BochtNaarLinks(int niveauVanBocht, int tegenDraai);
  void BochtNaarRechts(int niveauVanBocht, int tegenDraai);
  void AchterUit();
  

Servo servoL; //Voegt servo Links toe
Servo servoR; //Voegt servo Rechts toe
Servo arm;    //Voegt servo arm toe

//Alle globale variabelen
int sensor1 = A1;            //Variabelen voor de sensoren
int sensor2 = A2;   
int sensor3 = A0;
int sensor4 = A3;
int sensorWaarde1 = 0;       //Variabelen voor de sensorwaardes
int sensorWaarde2 = 0;
int sensorWaarde3 = 0;
int sensorWaarde4 = 0;
int rechtsOm = 1400;         //Servo draait tegen de klok in
int linksOm = 1600;          //Servo draait met de klok mee
int caseNumber;              //Variabele om de cases te zoeken
int threshold = 650;         //Threshold voor de sensor
int LED1 = 4;                //Welke led aan welke pin
int LED2 = 2;
int LED3 = 5;
int LED4 = 3;
int bochtVal;                //Variabele die de snelheid van de servo bepaalt 
int Sharp_sensor = A5;       //Sharp sensor welke pin
int Sharp_sensor_ledje = 8;  //Sharp sensor Ledje welke pin
int Sharp_sensor_waarde;     //variabele voor de waardes die worden uitgelezen door de sharp sensor

void setup() {
  // stelt de baudrate in zodat de seriele monitor werkt
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);     //Output voor de ledjes
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  servoL.attach(11);    //koppelt servoL aan pin 11
  servoR.attach(10);    //koppelt servoR aan pin 10
  arm.attach(12);       //koppelt de servo arm aan pin 12 
}

//Door: Ruben en Tristan
//De functies in de main loop
void loop() {
  SensorWaardeLezen();
  CasePicker(caseNumber);
  CaseFinder();
  Afstand_Sensor();
}

//Door: Stephan en Skip
//Deze functie zorgt ervoor dat een object wordt gedetecteerd en dat de servo arm gaat draaien
int Afstand_Sensor()
{
  Sharp_sensor_waarde = analogRead(Sharp_sensor);
  Serial.println(Sharp_sensor_waarde);

  if (Sharp_sensor_waarde > 400)
  {
    digitalWrite(Sharp_sensor_ledje, HIGH);
    Serial.println("STOP!!");
    servoL.writeMicroseconds(1500);
    servoR.writeMicroseconds(1500);

    arm.writeMicroseconds(2500);
    delay(6000);
    arm.writeMicroseconds(0);

    delay(2000);
  }
  else
  {
    digitalWrite(Sharp_sensor_ledje, LOW);
    delay(100);
  }
}

//Door Stephan
//Deze functie leest alle sensorwaardes op en zet ze in een ander variabele
void SensorWaardeLezen() {
  sensorWaarde1 = analogRead(sensor1);
  sensorWaarde2 = analogRead(sensor2);
  sensorWaarde3 = analogRead(sensor3);
  sensorWaarde4 = analogRead(sensor4);
}

//Door Ruben, Tristan, Stephan en Skip
//Deze functie kijkt naar de waardes die uit de sensoren komen en kiest daarbij de bijbehorende case
//Deze case stuurt de robot
void CasePicker(int welkeCase) {
  int i;
  
  switch (welkeCase) {
    //Sensor 0
    case 0:
      Serial.println("Padvolger is van de baan af zet hem terug");
      digitalWrite(LED1, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      // zorg er voor dat alle ledjes beginnen te knipper
      break;
    //Sensor 1
    case 1:
      Rechtdoor();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("case1");
      break;

    //Sensor 2
    case 2:
      BochtNaarLinks(-20, 100);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("case2");
      break;

    //sensor 1 en 2
    case 3:
      BochtNaarRechts(-20, -80);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      Serial.println("case3");
      break;

    //sensor 3
    case 4:
      Rechtdoor();
      digitalWrite(LED3, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED2, LOW);
      Serial.println("case4");
      break;

    //sensor 1 en 3
    case 5:
      Rechtdoor();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("case5");
      break;

    //sensor 2 en 3
    case 6:

      if ((welkeCase != 10) || (welkeCase != 11) || (welkeCase != 14) || (welkeCase != 15))
      {
      for (i = 0; i < 1000000; i++) {
        SensorWaardeLezen();
        BochtNaarLinks(-20, 100);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED1, LOW);
        digitalWrite(LED4, LOW);
        Serial.println("case6");
        if (sensorWaarde1  > threshold ) {
          break;
        }
      }
      }
      break;
    //sensor 1, 2 en 3
    case 7:
      Rechtdoor();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      Serial.println("case7");
      break;

    //sensor 4
    case 8:
      BochtNaarRechts(100, -100);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("case8");
      break;

    //sensor 1 en 4
    case 9:
      BochtNaarLinks(20, 80);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("case9");
      break;

    //sensor 2 en 4
    case 10:
      Rechtdoor();
      digitalWrite(LED2, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("case10");
      break;

    //sensor 1, 2, 4
    case 11:
      Rechtdoor();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED3, LOW);
      Serial.println("case11");
      break;

    //sensor  3, 4
    case 12:
      if ((welkeCase != 10) || (welkeCase != 11) || (welkeCase != 14) || (welkeCase != 15))
      {
      for (i = 0; i < 1000000; i++) {
        SensorWaardeLezen();
        BochtNaarRechts(20, -100);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        Serial.println("case12");
        if (sensorWaarde1  > threshold) {
          break;
        }
      }
      }
      break;

    //sensor 1, 3, 4
    case 13:
      Rechtdoor();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED2, LOW);
      Serial.println("case13");

      //Rechtdoor();
      break;

    //sensor  2, 3, 4
    case 14:
      Rechtdoor();
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED1, LOW);
      Serial.println("case14");
      break;

    //sensor 1, 2, 3, 4
    case 15:
      Rechtdoor();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      Serial.println("case15");
      break;

    //is onnodig maar laten we het toch maar doen
    default:
      //boebot gaat rechtdoor
      Rechtdoor();
      break;
  }
}

//Door Stephan
//Dit is onze manier om onze cases te vinden door middel van de waardes die we binnen krijgen van de sensoren. Zie Werking Software van het verslag
void CaseFinder() {
  caseNumber =
    (sensorWaarde1 > threshold) * 1 +
    (sensorWaarde2 > threshold) * 2 +
    (sensorWaarde3 > threshold) * 4 +
    (sensorWaarde4 > threshold) * 8;
}

//Door Ruben en Tristan
//Functie om de Boe-bot rechtdoor te laten rijden
void Rechtdoor() {
  int i;
  for (i = 0; i < 128; i++) {
    servoL.writeMicroseconds(1430);
    servoR.writeMicroseconds(1570);
  }
}

//Door Ruben en Tristan
//Functie om de Boe-bot een bocht naar links te laten maken
void BochtNaarLinks(int niveauVanBocht, int tegenDraai) {
  servoL.writeMicroseconds(1500 - tegenDraai);
  servoR.writeMicroseconds(1500 + niveauVanBocht);
}

//Door Ruben en Tristan
//Functie om de Boe-bot een bocht naar rechts te laten maken
void BochtNaarRechts(int niveauVanBocht, int tegenDraai) {
  servoL.writeMicroseconds(1500 + niveauVanBocht);
  servoR.writeMicroseconds(1500 - tegenDraai);
}

//Door Ruben en Tristan
//Functie om de Boe-bot rechtdoor te laten rijden
void AchterUit() {
  volatile int i;
  for (i = 0; i < 1; i++) {
    servoL.writeMicroseconds(1550);
    servoR.writeMicroseconds(1450);
  }
}
