const int DELAY = 1000;

void setup(void) 
{
    Serial1.begin(115200);
}

void loop() 
{
    Serial1.println("Hello world");
    delay(DELAY);
}
