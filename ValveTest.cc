#include <wiringPi.h>

#define PIN_ON  28
#define PIN_OFF 29
int main (void)
{
  wiringPiSetup () ;
  pinMode (PIN_ON, OUTPUT);
  pinMode (PIN_OFF, OUTPUT);

  for (;;)
  {
    digitalWrite (PIN_ON, HIGH) ; delay (100) ;
    digitalWrite (PIN_ON,  LOW) ; delay (2000) ;
    digitalWrite (PIN_OFF, HIGH) ; delay (100) ;
    digitalWrite (PIN_OFF,  LOW) ; delay (2000) ;
  }
  return 0 ;
}
