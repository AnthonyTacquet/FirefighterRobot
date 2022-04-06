// anthony.tacquet@gmail.com
// ***********************************************************************************************
#define WATCHDOG
#define SPEED 9600
#define TURNDELAY 100
#define ROTATEDELAY 1000

#include <Wire.h>
//#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#include <ArduinoJson.h>
StaticJsonDocument<256> doc;

// *******************
// TIMER
// ********************
#define DELAY1MS       1
#define DELAY10MS      10
#define DELAY1S        1000
#define DELAY2S        2000
#define DELAY10S       10000
#define DELAY1M        30000
#define DELAY10M       600000
#define DELAY1H        3600000
//auto timer = timer_create_default();
#include <Timer.h>
Timer timer;
bool ok = true;
int i= 0;
int flag = 0; 
int LED = A0;
int state = 0;

Servo Servo1;
Servo Servo2;
int servoPin1 = 13; 
int servoPin2 = 12;


int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
char val;
// ********************
// TIMER
// ********************
void timer_init()
{
  Serial.println("timer init");
}

// ********************
// PORT 
// ********************
void port_init()
{  
  Serial.begin(9600);
  Serial3.begin(SPEED);  
  Serial.println("port init");

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
}

void servo_init()
{
  Serial.println("servo init");
  Servo1.attach(servoPin1); 
  Servo2.attach(servoPin2);
}
// ***********************************************************************************************
void servo_turn(bool right)
{
  int where = Servo1.read();
  right?where+=10:where-=10;
  int new_servo_pos = max(5,min(where,157));
  Servo1.write(new_servo_pos);
}

void servo_turn2(bool up)
{
  int here = Servo2.read();
  up?here+=10:here-=10;
  int new_servo_pos2 = max(5,min(here,157));
  Servo2.write(new_servo_pos2);
}

void leftengine(int state = 0)
{
  if (state == 1) {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
  } else if (state == 2) {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
  } else {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);    
  } 
}

void rightengine(int state = 0)
{
  if (state == 1) {
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
  } else if (state == 2) {
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
  } else {
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);    
  } 
}

void forward() {
  leftengine(1);
  rightengine(1); 
}

void backward() {
  leftengine(2);
  rightengine(2); 
}

void fastleft() {
  leftengine(2);
  rightengine(1); 
}

void fastright() {
  leftengine(1);
  rightengine(2); 
}

void nomotion() {
  leftengine(0);
  rightengine(0); 
}

// ***********************************************************************************************
void setup()
{
  port_init();
  interrupt_init();
  timer_init();
  servo_init();
  Serial.println("ready");
}
// ***********************************************************************************************
void loop()
{
  //Timer();
  if (Serial3.available()) 
  {
    String text;
    while (Serial3.available()) 
    {
      char data = Serial3.read();
      text = String(text + data);
    }
    bool A,B,C,D,P,T;
    bool L,R,W,U;
    A=text=="a";
    B=text=="b";
    C=text=="c";
    D=text=="d";
    P=text=="stop";
    T=text=="start";

    L=text=="left";
    R=text=="right";
    W=text=="down";
    U=text=="up";

    Serial.println("+abcd+xytp+");
    Serial.println("-----------");
    Serial.print('|'); 
    A?Serial.print("A"):Serial.print("_"); 
    B?Serial.print("B"):Serial.print("_"); 
    C?Serial.print("C"):Serial.print("_"); 
    D?Serial.print("D"):Serial.print("_"); 
    Serial.print('|');
    (L)?Serial.print(char(16)):Serial.print("");
    (R)?Serial.print(char(17)):Serial.print("");
    (!L&&!R)?Serial.print("_"):Serial.print("");
    (U)?Serial.print(char(19)):Serial.print("");
    (W)?Serial.print(char(20)):Serial.print("");
    (!W&&!U)?Serial.print("_"):Serial.print("");
    T?Serial.print("T"):Serial.print("_");    
    P?Serial.print("P"):Serial.print("_");
    Serial.print('|');        
    Serial.println();

    if (A)
      servo_turn(true);

    if (B)
      servo_turn(false);

    if (C)
      servo_turn2(true);

    if (D)
      servo_turn2(false);

    if (U) {
      forward();
    } else if (W) {
      backward();
    } else if (L) {
      fastleft();
      delay(TURNDELAY);
      nomotion();
    } else if (R) {
      fastright();
      delay(TURNDELAY);
      nomotion();
    } else if (T) {
      fastleft();
      delay(ROTATEDELAY);
      nomotion();
    } else if (P) {
      nomotion();
    }
  }
  delay(100);
}

void interrupt_init()
{
  Serial.println("interrupt init");

}


// ************************************************************************************************
