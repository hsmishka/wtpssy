#include <wiringPi.h>

class ValveCtrlr
{
	int gpio_valve_on;
	int gpio_valve_off;
	
	static ValveCtrlr& getInstance()
	{
		static ValveCtrlr instance;
		return instance;
	}
    private:
        ValveCtrlr(int gpio_valve_on, int gpio_valve_off) {};
        ValveCtrlr(ValveCtrlr const&);     // Don't Implement
        void operator=(ValveCtrlr const&); // Don't implement

    public:
        ValveCtrlr(ValveCtrlr const&)      = delete;
        void operator=(ValveCtrlr const&)  = delete;

		
};

int main (void)
{
  wiringPiSetup () ;
  pinMode (0, OUTPUT) ;
  for (;;)
  {
    digitalWrite (0, HIGH) ; delay (500) ;
    digitalWrite (0,  LOW) ; delay (500) ;
  }
  return 0 ;
}
