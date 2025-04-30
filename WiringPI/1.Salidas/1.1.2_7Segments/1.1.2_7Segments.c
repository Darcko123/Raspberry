#include <wiringPi.h>
#include <stdio.h>

#define PinA      8
#define PinB      9
#define PinC      0
#define PinD      12
#define PinE      30
#define PinF      21
#define PinG      22

void One(void);
void Two(void);
void Three(void);
void Four(void);
void Five(void);
void Six(void);
void Seven(void);
void Eight(void);
void Nine(void);
void Zero(void);

int main(void)
{
   // When initialize wiring failed, print message to screen
   if(wiringPiSetup() == -1){
      printf("setup wiringPi failed !");
      return 1;
   }
   
   pinMode(PinA, OUTPUT);
   pinMode(PinB, OUTPUT);
   pinMode(PinC, OUTPUT);
   pinMode(PinD, OUTPUT);
   pinMode(PinF, OUTPUT);
   pinMode(PinG, OUTPUT);

   while(1)
   {
      One();   printf("One\n");    delay(3000);
      Two();   printf("Two\n");    delay(3000);
      Three(); printf("Three\n");  delay(3000);
      Four();  printf("Four\n");   delay(3000);
      Five();  printf("Five\n");   delay(3000);
      Six();   printf("Six\n");    delay(3000);
      Seven(); printf("Seven\n");  delay(3000);
      Eight(); printf("Eight\n");  delay(3000);
      Nine();  printf("Nine\n");   delay(3000);
      Zero();  printf("Zero\n");   delay(3000);
   }
   return 0;
}

void One(void)
{
   digitalWrite(PinA, LOW);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, LOW);
   digitalWrite(PinE, LOW);
   digitalWrite(PinF, LOW);
   digitalWrite(PinG, LOW);
}

void Two(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, LOW);
   digitalWrite(PinD, HIGH);
   digitalWrite(PinE, HIGH);
   digitalWrite(PinF, LOW);
   digitalWrite(PinG, HIGH);
}

void Three(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, HIGH);
   digitalWrite(PinE, LOW);
   digitalWrite(PinF, LOW);
   digitalWrite(PinG, HIGH);
}
void Four(void)
{
   digitalWrite(PinA, LOW);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, LOW);
   digitalWrite(PinE, LOW);
   digitalWrite(PinF, HIGH);
   digitalWrite(PinG, HIGH);
}
void Five(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, LOW);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, HIGH);
   digitalWrite(PinE, LOW);
   digitalWrite(PinF, HIGH);
   digitalWrite(PinG, HIGH);
}
void Six(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, LOW);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, HIGH);
   digitalWrite(PinE, HIGH);
   digitalWrite(PinF, HIGH);
   digitalWrite(PinG, HIGH);
}
void Seven(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, LOW);
   digitalWrite(PinE, LOW);
   digitalWrite(PinF, LOW);
   digitalWrite(PinG, LOW);
}
void Eight(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, HIGH);
   digitalWrite(PinE, HIGH);
   digitalWrite(PinF, HIGH);
   digitalWrite(PinG, HIGH);
}
void Nine(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, LOW);
   digitalWrite(PinE, LOW);
   digitalWrite(PinF, HIGH);
   digitalWrite(PinG, HIGH);
}
void Zero(void)
{
   digitalWrite(PinA, HIGH);
   digitalWrite(PinB, HIGH);
   digitalWrite(PinC, HIGH);
   digitalWrite(PinD, HIGH);
   digitalWrite(PinE, HIGH);
   digitalWrite(PinF, HIGH);
   digitalWrite(PinG, LOW);
}