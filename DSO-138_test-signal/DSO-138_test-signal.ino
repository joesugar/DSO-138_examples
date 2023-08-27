// DSO-138 sample code
// Demonstrating how to set up the test signal
//
// Test signal is attached to PIN 17 (PA7) on the STM32F103
//

#define TEST_SIGNAL PA7
#define PRESCALER   72
#define OVERFLOW    1000
#define DUTY_CYCLE  500

void setup() 
{
    // Set the pin mode to PWM to get the repeating signal.
    // This way we won't have to change the level ourselves.
    //
    pinMode(TEST_SIGNAL, PWM);
    
    // Timer prescale of 72 changes the timer clock to 1 MHz.
    // Overflow set to 1000 gives us a 1 kHz signal.
    // Duty cycle set to 500 gives us a 50% duty cycle.
    //
    Timer3.setPrescaleFactor(PRESCALER);
    Timer3.setOverflow(OVERFLOW);
    pwmWrite(TEST_SIGNAL, DUTY_CYCLE);
}

void loop() 
{

}
