#include <wiringPi.h>
#include <stdio.h>

#define LedPin      8

int main(void)
{
   // When initialize wiring failed, print message to screen
   if(wiringPiSetup() == -1){
      printf("setup wiringPi failed !");
      return 1;
   }
   
   pinMode(LedPin, OUTPUT);// Set LedPin as output to write value to it.

   while(1)
   {
      // LED on
      digitalWrite(LedPin, LOW);
      printf("...LED on\n");
      delay(500);
      // LED off
      digitalWrite(LedPin, HIGH);
      printf("LED off...\n");
      delay(500);
   }
   return 0;
}
