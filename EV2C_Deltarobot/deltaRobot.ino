
// #################################################
// ##             code for movement               ##
// ##          the delta-robot project            ##
// ##               for Prof 3C                   ##
// ##              code writen by                 ##
// ##            Barend van der Ven               ##
// ##             Ruben Middelman                 ##
// ##               Used by the                   ##
// ##      HU University of Applied Sciences'     ##
// #################################################
#include <math.h>
#include<Servo.h>

//makes the servo's from the servo librairy
Servo servo1;
Servo servo2;
Servo servo3;

const int magnetPin = 8;

//constant variables that where measured from the delta-robot
const int Larm2 = 20;
const int Larm1 = 5;
const int Ltop = 5;
const int Lbase = 10;

//these variables are for controlling the delta-robot
float Aansturing_servo1 = 193;
float Aansturing_servo2 = 193;
float Aansturing_servo3 = 193;


//variable wth the coordinates
int doel[3];
int interfaceMedicinNumber = 0;
int delayTime = 200;

//makes the struct for the servo's
struct Bstuur_servo_aan{
  int servo1Hoek ;
  int servo2Hoek ;
  int servo3Hoek ;
};
//struct for the calling of the servos's
struct Bstuur_servo_aan stuur_servo_aan;



void setup()
{
  //sets all the servo's to the correct pins
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  //sets the servo's to a default position
  servo1.write(200);
  servo2.write(200);
  servo3.write(200);

  //is used for the magnet
  pinMode(magnetPin, OUTPUT);
  //sets up the serial comminucation that is used by the interface
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void loop()
{
  SerialChecker ();
}


void CoordinatesToAnglesCalculator()
//servo 1
{
  //makes all the variables for the function that calculates the angles that are used by the servo's
  int hoek = 1;

  int Xnull = 0;
  int Ynull = 0;
  int Znull = 0;

  float Xpols;
  float Ypols;
  float Zpols;

  float Xelleboog;
  float Yelleboog;
  float Zelleboog;

  float Xservo;
  float Yservo;
  float Zservo;
  //calculates the closest angle to the coordinate with a main
  //makes a LOT of calculations so can be a bit slow on some systems
  for (hoek = 0; hoek < 125 ; hoek++)
  {
    //does all the math to go from the coordinates to the angles but only for servo 1
    Xpols = doel[0] + Ltop;
    Ypols = doel[1];
    Zpols = doel[2];

    Xservo = Xnull + Lbase;
    Yservo = Ynull;
    Zservo = Znull;

    Xelleboog = Xservo + Larm1 * cos(((hoek) * PI) / 180);
    Yelleboog = Yservo;
    Zelleboog = Zservo + Larm1 * sin(((hoek) * PI) / 180);

    Afstand_1 = sqrt(sq(Xpols - Xelleboog) + sq(Ypols - Yelleboog) + sq(Zpols - Zelleboog));

    if ((Afstand_1 < 20.1) && (Afstand_1 > 19.90))
    {
      //this is the closest angle that was found
      Aansturing_servo1 = 180 - hoek;
    }
    //does all the math to go from the coordinates to the angles but only for servo 2
    //servo2
    Xpols = doel[0] - cos(60 * PI / 180) * Ltop;
    Ypols = doel[1] + sin(60 * PI / 180) * Ltop;
    Zpols = doel[2];

    Xservo = Xnull - cos(60 * PI / 180) * Lbase;
    Yservo = Ynull + sin(60 * PI / 180) * Lbase;
    Zservo = Znull;

    Xelleboog = Xservo - cos(60 * PI / 180) * Larm1 * cos(((hoek) * PI) / 180);
    Yelleboog = Yservo + sin(60 * PI / 180) * Larm1 * cos(((hoek) * PI) / 180);
    Zelleboog = Zservo + Larm1 * sin((hoek) * PI / 180);

    Afstand_2 = sqrt(sq(Xpols - Xelleboog) + sq(Ypols - Yelleboog) + sq(Zpols - Zelleboog));
    if ((Afstand_2 < 20.1) && (Afstand_2 > 19.90))
    {
      Aansturing_servo2 = 180 - hoek;
    }
    //servo3
    //does all the math to go from the coordinates to the angles but only for servo 3
    Xpols = doel[0] - cos(60 * PI / 180) * Ltop;
    Ypols = doel[1] - sin(60 * PI / 180) * Ltop;
    Zpols = doel[2];

    Xservo = Xnull - cos(60 * PI / 180) * Lbase;
    Yservo = Ynull - sin(60 * PI / 180) * Lbase;
    Zservo = Znull;

    Xelleboog = Xservo - cos(60 * PI / 180) * Larm1 * cos(((hoek) * PI) / 180);
    Yelleboog = Yservo - sin(60 * PI / 180) * Larm1 * cos(((hoek) * PI) / 180);
    Zelleboog = Zservo + Larm1 * sin((hoek) * PI / 180);

    Afstand_3 = sqrt(sq(Xpols - Xelleboog) + sq(Ypols - Yelleboog) + sq(Zpols - Zelleboog));

    if ((Afstand_3 < 20.1) && (Afstand_3 > 19.90))
    {
      Aansturing_servo3 = 180 - hoek;
    }
  }
}

void MoveServo()
{
  //moves the servo and waits for half a second
  servo1.write(stuur_servo_aan.servo1Hoek);
  servo2.write(stuur_servo_aan.servo2Hoek);
  servo3.write(stuur_servo_aan.servo3Hoek);

  delay(500);
}

//function that goes through the coordinates 1 by 1
void SerialChecker(){
  //gets the data via serial from the laptop and converts it to an integer
  while (!Serial.available());
  interfaceMedicinNumber = Serial.readString().toInt();
  Serial.print(interfaceMedicinNumber);
  //if input from interface == 0 then do call the MoveMedicin function that
  //has all the coordinates
  if (interfaceMedicinNumber == 0) {
    MoveMedicin();
  }
  //used for debugging and testing
  if (interfaceMedicinNumber == 2) {
    TurnOnMagnet();
  }
  if (interfaceMedicinNumber == 3) {
    TurnOffMagnet();
  }
}

void TurnOnMagnet() {
  digitalWrite(magnetPin, HIGH);
}
void TurnOffMagnet() {
  digitalWrite(magnetPin, LOW);
}

void MoveMedicin(){
  //has all the coordinates for the robot
  //is coded in a very ugly manner but because of deadline not fixed
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 22;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  TurnOnMagnet();
  delay(delayTime);
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 22;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  TurnOffMagnet();
  delay(delayTime);
  TurnOffMagnet();
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 22;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  TurnOnMagnet();
  delay(delayTime);
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 22;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  TurnOffMagnet();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 22;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  TurnOnMagnet();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 6;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 6;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = -6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = -6;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  doel[0] = 6;
  doel[1] = 0;
  doel[2] = 22;
  CoordinatesToAnglesCalculator();
  MoveServo();
  delay(delayTime);
  TurnOffMagnet();
  delay(delayTime);
  doel[0] = 0;
  doel[1] = 0;
  doel[2] = 18;
  CoordinatesToAnglesCalculator();
  MoveServo();
}
