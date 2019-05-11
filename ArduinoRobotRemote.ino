#include <IRremote.h>
#include <IRremoteInt.h>
#include <Servo.h>

long duration;
float distance, RightSensor, FrontSensor, LeftSensor;
IRrecv irrecv(53);
decode_results results;
uint32_t prev;
Servo base;
Servo claw;
Servo right;
Servo left;
int posb = 90;
int posc = 90;
int posr = 90;
int posl = 90;
#define trigR 31
#define trigC 32
#define trigL 33
#define echR  41
#define echC  42
#define echL  43
const int E1 = 3; 
const int E2 = 11;
const int E3 = 5;
const int E4 = 6;
const int M1 = 4; 
const int M2 = 12;
const int M3 = 8; 
const int M4 = 7;

void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
}

void motor_forward(int Speedx){
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
  analogWrite(E1, Speedx);
  analogWrite(E2, Speedx);
  analogWrite(E3, Speedx);
  analogWrite(E4, Speedx);
}

void motor_reverse(int Speedx){
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
  analogWrite(E1, Speedx);
  analogWrite(E2, Speedx);
  analogWrite(E3, Speedx);
  analogWrite(E4, Speedx);
}

void motor_right(int Speedx){
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, LOW);
  analogWrite(E1, Speedx);
  analogWrite(E2, Speedx);
  analogWrite(E3, Speedx);
  analogWrite(E4, Speedx);
}

void motor_left(int Speedx){
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, HIGH);
  analogWrite(E1, Speedx);
  analogWrite(E2, Speedx);
  analogWrite(E3, Speedx);
  analogWrite(E4, Speedx);
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(trigR,OUTPUT);
  pinMode(trigC,OUTPUT);
  pinMode(trigL,OUTPUT);
  pinMode(echR, INPUT);
  pinMode(echC, INPUT);
  pinMode(echL, INPUT);
  pinMode(30, OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  base.attach(2);
  claw.attach(13);
  right.attach(9);
  left.attach(10);
  base.write(90);
  claw.write(90);
  right.write(90);
  left.write(90);    
}


void loop() {
  if(irrecv.decode(&results)){

    if(results.value==16580863){      ////   FIRST STATEMENT

      while(results.value==16580863){
      
      //Serial.println("checkpoint good");

              SonarSensor(trigC,echC);
                if (distance>=50){
                        distance=50;
                }
                FrontSensor = distance;
                
                SonarSensor(trigR,echR);
                if (distance>=50){
                        distance=50;
                }
                RightSensor = distance;
                
                SonarSensor(trigL,echL);
                if (distance>=50){
                        distance=50;
                }
                LeftSensor = distance;

                //Serial.println(RightSensor);
/**/
                      if (FrontSensor>15){
                              digitalWrite(30, HIGH);
                              Serial.println("moving forward");
                              motor_forward(150);
                              digitalWrite(30, LOW);
                            }
                              else if (FrontSensor<=17){
                                if((LeftSensor>=RightSensor)){
                                digitalWrite(30, HIGH);
                                Serial.println("moving left");
                                //motor_forward(150);
                                //delay(250);
                                motor_left(150);
                                delay(220);
                                digitalWrite(30, LOW);
                              }
                              else if((RightSensor>LeftSensor)){
                                digitalWrite(30, HIGH);
                                Serial.println("moving right");
                                //motor_forward(150);
                                //delay(250);
                                motor_right(150);
                                delay(220);
                                digitalWrite(30, LOW);
                              }
                            }

/**/
                            irrecv.resume();
                            delay(100);


                            if(irrecv.decode(&results)){
                            if(results.value!=16580863)
                            {
                              break;
                            }
                            }
                            
      }

      
      irrecv.resume();
    }


    
    else if(results.value!=16580863){ ////  NEXT STATEMENT

                              motor_forward(0);
                              if(results.value==4294967295){   
                                       results.value = prev;
                              }
                              switch(results.value){
                              case 0xFD30CF:      Serial.println("MachineStop"); break;
                              case 0xFD08F7:      /*if(posl<=0){posl==0;break;};*/ left.write(posl=posl-10); Serial.println("A"); break;
                              case 0xFD8877:      /*if(posc<=0){posc==0;break;};*/ claw.write(posc=posc-10); Serial.println("B"); break;
                              case 0xFD48B7:      /*if(posr<=0){posr==0;break;};*/ right.write(posr=posr-10); Serial.println("C"); break;
                              case 0xFD28D7:      /*if(posl>=1800){posl==180;break;};*/ left.write(posl=posl+10); Serial.println("D"); break;
                              case 0xFDA857:      /*if(posc>=180){posc==180;break;};*/ claw.write(posc=posc+10); Serial.println("E"); break;
                              case 0xFD6897:      /*if(posr>=180){posr==180;break;};*/ right.write(posr=posr+10); Serial.println("F"); break;
                              case 0xFD18E7:      /*if(posb<=0){posb==0;break;};*/ base.write(posb=posb-10); Serial.println("G"); break;
                              case 0xFD9867:      posb=0;posc=90;posr=0;posl=0;base.write(90);claw.write(90);left.write(90);right.write(90);  Serial.println("H"); break;
                              case 0xFD58A7:      /*if(posb>=180){posb==180;break;};*/ base.write(posb=posb+10); Serial.println("I");  break;
                              }
                              prev = results.value;




      
      irrecv.resume();
    }

    
    irrecv.resume();
    //Serial.println(results.value);
  }
}

//MITL
