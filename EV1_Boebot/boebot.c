#include <Servo.h>

Servo servoL; // voegt servo Links toe.
Servo servoR; // voegt servo Rechts toe.

int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensorWaarde1 = 0;
int sensorWaarde2 = 0;
int sensorWaarde3 = 0;
int sensorWaarde4 = 0;
float vertrager = 0.1;
int rechtsOm = 1400;
int linksOm = 1600;
int caseNumber;
int threshold = 264;
int LED1 = 4;
int LED2 = 2;
int LED3 = 3;
int LED4 = 5;
int bochtVal;

void setup() {
  // stelt de baudrate in zodat de seriele monitor werkt
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  servoL.attach(11);    //koppelt servoL aan pin 11
  servoR.attach(10);    //koppelt servoR aan pin 10
}

void loop() {
  SensorWaardeLezen();
  CasePicker(caseNumber);
  CaseFinder();
}


//leest alle sensorwaardes op en zet ze in een ander variable
void SensorWaardeLezen() {
  sensorWaarde1 = analogRead(sensor1);
  sensorWaarde2 = analogRead(sensor2);
  sensorWaarde3 = analogRead(sensor3);
  sensorWaarde4 = analogRead(sensor4);
}


void CasePicker(int welkeCase) {
  switch (welkeCase) {
    //Case 0 is als geen een sensor iets uitlaat
    case 0:
      AchterUit();
      Serial.println("Padvolger is van de baan af zet hem terug");

      digitalWrite(LED1, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      // zorg er voor dat alle ledjes beginnen te knipper
      break;
    //Case 1 is als sensor 1 hoog uitslaat
    case 1:
      // zorg dat de robot recht door blijft gaan
      Serial.println("case1");
      BochtNaarLinks(-30, 30);
      break;

    //case 2 is als sensor 2 hoog uitslaat
    case 2:
      //zorg dat boebot scherp naar links gaat
      Serial.println("case2");
      BochtNaarLinks(-20, 40);
      break;

    //sensor 3
    case 3:
      //bocht naar rechts
      Serial.println("case3");
      BochtNaarRechts(30, -20);
      break;

    //sensor 4
    case 4:
      // boebot gaat zeer scherp naar rechts
      Serial.println("case4");
      BochtNaarRechts(-10, -30);
      break;

    //sensor 1 en 2
    case 5:
      // maak een bocht naar links
      Serial.println("case5");
      BochtNaarRechts(-20, -40);
      break;

    //sensor 1 en 3
    case 6:
      // boebot gaat rechtdoor
      Serial.println("case6");
      Rechtdoor();
      break;

    //sensor 1 en 4
    case 7:
      //slappe bocht naar recht
      Serial.println("case7");
      BochtNaarLinks(-20, 40);
      break;

    //sensor 2 en 3
    case 8:
      //heel erg scherp naar links
      Serial.println("case8");
      BochtNaarLinks(10, 30);
      break;

    //sensor 2 en 4
    case 9:
      // op hoop van zegen de boebot 90 graden draaien
      Serial.println("case9");
      BochtNaarRechts(40, -40);
      break;

    //sensor 3 en 4
    case 10:
      //zeer scherpe bocht naar rechts
      Serial.println("case10");
      BochtNaarRechts(10, -40);
      break;

    //sensor 1, 2, 3
    case 11:
      // boebot moet rechtdoor
      Serial.println("case11");
      Rechtdoor();
      break;

    //sensor 2, 3, 4
    case 12:
      //op hoop van zegen boebot 90 grade draaien
      Serial.println("case12");
      BochtNaarRechts(40, -40);
      break;

    //sensor 1, 3, 4
    case 13:
      //boebot moet rechtdoor
      Serial.println("case13");
      Rechtdoor();
      break;

    //sensor 1, 2, 4
    case 14:
      //boebot gaat rechtdoor
      Serial.println("case14");
      Rechtdoor();
      break;

    //sensor 1, 2, 3, 4
    case 15:
      Serial.println("case15");
      //boebot gaat rechtdoor
      Rechtdoor();
      break;

    //is onnodig maar laten we het toch maar doen
    default:
      //boebot gaat rechtdoor
      Rechtdoor();
      break;
  }
}

void CaseFinder() {
  //just a bunch of if statements that find which case should be used
  if (sensorWaarde1 > 100) {
    caseNumber = 1;
    digitalWrite(LED1, HIGH);
    digitalWrite(LED4, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    if (sensorWaarde2 > threshold) {
      caseNumber = 5;
      digitalWrite(LED1, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      if (sensorWaarde3 > threshold) {
        caseNumber = 11;
        digitalWrite(LED1, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        if (sensorWaarde4 > threshold) {
          caseNumber = 15;
          digitalWrite(LED1, HIGH);
          digitalWrite(LED4, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
        }
      } else if (sensorWaarde4 > threshold) {
        caseNumber = 14;
      }
    }
    if (sensorWaarde3 > threshold) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      caseNumber = 6;
      if (sensorWaarde4 > threshold) {
        caseNumber = 13;
      }
    }
    if (sensorWaarde4 > threshold) {
      caseNumber = 7;
    }
  } else if (sensorWaarde2 > threshold) {
    caseNumber = 2;
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED3, LOW);
    if (sensorWaarde3 > threshold) {
      caseNumber = 8;
      digitalWrite(LED1, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      if (sensorWaarde4 > threshold) {
        caseNumber = 12;
      }
    }
    if (sensorWaarde4 > threshold) {
      caseNumber = 9;
    }
  } else if (sensorWaarde3 > threshold) {
    caseNumber = 3;
    digitalWrite(LED3, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED4, HIGH);
    if (sensorWaarde4 > threshold) {
      caseNumber = 10;
      digitalWrite(LED1, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
    }
  } else if (sensorWaarde4 > threshold) {
    caseNumber = 4;
    digitalWrite(LED4, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  } else {
    caseNumber = 0;
    digitalWrite(LED1, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }

}

void Rechtdoor() {
  int a;
  for (a = 0; a < 8; a++) {
    servoL.writeMicroseconds(rechtsOm);
    servoR.writeMicroseconds(linksOm);
  }
}

void BochtNaarLinks(int niveauVanBocht, int tegenDraai) {
  servoL.writeMicroseconds(1500 - tegenDraai);
  servoR.writeMicroseconds(1500 + niveauVanBocht);
}

void BochtNaarRechts(int niveauVanBocht, int tegenDraai) {
  servoL.writeMicroseconds(1500 + niveauVanBocht);
  servoR.writeMicroseconds(1500 - tegenDraai);
}

void AchterUit() {
  volatile int i;
  for (i = 0; i < 1; i++) {
    servoL.writeMicroseconds(linksOm);
    servoR.writeMicroseconds(rechtsOm);
  }
}
