// LED definition
//
const int LED_PIN = PA15;     // DSO-138

// Button definitions
//
const uint8 SW4 = PB15;
const uint8 SW5 = PB14;
const uint8 SW6 = PB13;
const uint8 SW7 = PB12;

void setup() 
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(SW4, INPUT_PULLUP);  
    pinMode(SW5, INPUT_PULLUP);  
    pinMode(SW6, INPUT_PULLUP);  
    pinMode(SW7, INPUT_PULLUP);  
}

void loop() 
{
    static boolean LED_ON = LOW;
    static boolean LED_OFF = HIGH;
    
    boolean sw4 = digitalRead(SW4);   // read the input pins.  Normally hi.
    boolean sw5 = digitalRead(SW5);
    boolean sw6 = digitalRead(SW6);
    boolean sw7 = digitalRead(SW7);

    boolean output = LED_OFF;         // If any of them are low light the LED
    if (sw4 == LOW) output = LED_ON;
    if (sw5 == LOW) output = LED_ON;
    if (sw6 == LOW) output = LED_ON;
    if (sw7 == LOW) output = LED_ON;
    digitalWrite(LED_PIN, output);
}
