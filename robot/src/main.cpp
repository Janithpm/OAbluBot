#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>

#define in_1 2
#define in_2 4
#define in_3 5
#define in_4 7
#define enA 3
#define enB 6
#define servoPin A2
#define pushButtonPin 11
#define led_1 13
#define led_2 2
#define trigPin A1
#define echoPin 12
#define servoRight 0
#define servoMiddle 80
#define servoLeft 180
#define maxDistance 250
#define gap 50
#define servoDelay 400
#define motorDelay 500
#define sensorDelay 150

int spdA = 100;
int spdB = 252;
int distance, distanceL, distanceR;
int cmd = 0;
bool RCMode = false;
bool autoPMode = false;
bool pushButtonStatues = false;

Servo servoMotor;
NewPing carSonar(trigPin, echoPin);

void setup()
{
  Serial.begin(9600);
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  servoMotor.attach(servoPin);
  startup();
}

void loop()
{
  if (digitalRead(pushButtonPin) == 1)
  {
    autoPMode = !autoPMode;
    delay(200);
  }
  if (Serial.available())
  {
    cmd = Serial.read();
    //Serial.println(cmd);
    if (cmd == 115)
    {
      RCMode = false;
      autoPMode = false;
    }
    else if (cmd == 114)
    {
      RCMode = true;
      autoPMode = false;
    }
    else if (cmd == 97)
    {
      RCMode = false;
      autoPMode = true;
    }
  }
  else
  {
    cmd = 0;
    //    stopMove();
  }

  if (RCMode)
  {
    RC_mode();
  }
  if (autoPMode)
  {
    autoPilot();
  }
}

void startup()
{
  servoMotor.write(servoMiddle);
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(led_1, HIGH);
    delay(100);
    digitalWrite(led_1, LOW);
    delay(100);
  }
}

int readSonar()
{
  int tempDist = carSonar.ping_cm();
  if (tempDist == 0)
  {
    tempDist = maxDistance;
  }
  delay(50);
  return tempDist;
}

void stopMove()
{
  Serial.println("stop");
  digitalWrite(in_1, LOW); //right
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, LOW); //left
  digitalWrite(in_4, LOW);
}

void moveForward()
{
  //  Serial.println("moveForward");
  digitalWrite(in_1, LOW);
  digitalWrite(in_2, HIGH);
  digitalWrite(in_3, LOW);
  digitalWrite(in_4, HIGH);
  analogWrite(enA, spdA);
  analogWrite(enB, spdB);
}

void moveBackward()
{
  //  Serial.println("moveBackward");
  digitalWrite(in_1, HIGH);
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, HIGH);
  digitalWrite(in_4, LOW);
  analogWrite(enA, 160);
  analogWrite(enB, spdB);
}
void turnLeft()
{
  //  Serial.println("turnLeft");
  digitalWrite(in_1, LOW);
  digitalWrite(in_2, HIGH);
  digitalWrite(in_3, HIGH);
  digitalWrite(in_4, LOW);
  analogWrite(enA, spdA);
  analogWrite(enB, spdB);
}

void turnRight()
{
  //  Serial.println("turnRight");
  digitalWrite(in_1, HIGH);
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, LOW);
  digitalWrite(in_4, HIGH);
  analogWrite(enA, spdA);
  analogWrite(enB, spdB);
}

void leftForward()
{
  //  Serial.println("leftForward");
  digitalWrite(in_1, LOW);
  digitalWrite(in_2, HIGH);
  digitalWrite(in_3, LOW);
  digitalWrite(in_4, HIGH);
  analogWrite(enA, spdA);
  analogWrite(enB, spdB / 4);
}

void rightForward()
{
  //  Serial.println("rightForward");
  digitalWrite(in_1, LOW);
  digitalWrite(in_2, HIGH);
  digitalWrite(in_3, LOW);
  digitalWrite(in_4, HIGH);
  analogWrite(enA, spdA / 4);
  analogWrite(enB, spdB);
}

void leftBackward()
{
  //  Serial.println("leftBackward");
  digitalWrite(in_1, HIGH);
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, HIGH);
  digitalWrite(in_4, LOW);
  analogWrite(enA, spdA);
  analogWrite(enB, spdB / 4);
}

void rightBackward()
{
  //    Serial.println("rightBackward");
  digitalWrite(in_1, HIGH);
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, HIGH);
  digitalWrite(in_4, LOW);
  analogWrite(enA, spdA / 4);
  analogWrite(enB, spdB);
}

void RC_mode()
{
  if (cmd == 0)
  {
    stopMove();
  }
  if (cmd == 49)
  {
    moveForward();
  }
  if (cmd == 50)
  {
    moveBackward();
  }
  if (cmd == 51)
  {
    turnLeft();
  }
  if (cmd == 52)
  {
    turnRight();
  }
  if (cmd == 53)
  {
    leftForward();
  }
  if (cmd == 54)
  {
    rightForward();
  }
  if (cmd == 55)
  {
    leftBackward();
  }
  if (cmd == 56)
  {
    rightBackward();
  }
}

int lookLeftSide()
{
  servoMotor.write(servoLeft);
  delay(servoDelay);
  int tempDist = readSonar();
  delay(sensorDelay);
  servoMotor.write(servoMiddle);
  return tempDist;
}

int lookRightSide()
{
  servoMotor.write(servoRight);
  delay(servoDelay);
  int tempDist = readSonar();
  delay(sensorDelay);
  servoMotor.write(servoMiddle);
  return tempDist;
}

void autoPilot()
{
  distance = readSonar();
  if (distance <= gap)
  {
    stopMove();
    delay(sensorDelay);
    distanceR = lookRightSide();
    distanceL = lookLeftSide();
    while (distanceR <= gap && distanceL <= gap)
    {
      moveBackward();
      delay(motorDelay);
      stopMove();
      distanceR = lookRightSide();
      distanceL = lookLeftSide();
    }
    if (distanceL >= distanceR)
    {
      turnLeft();
      delay(motorDelay);
      stopMove();
    }
    else
    {
      turnRight();
      delay(800);
      stopMove();
    }
  }
  else
  {
    moveForward();
  }
}
