#include <STM32ADC.h>

#define SWITCH1        PA1    // SEN2
#define SWITCH2        PA2    // SEN1
#define SWITCH3        PA3    // CPL

#define TEST_WAVE_PIN  PA7    // 1KHz square wave output

#define TRIGGER_LEVEL  PB8
#define VGEN           PB9    // used to generate negative voltage

#define SWITCH_LOW     0
#define SWITCH_MIDDLE  1
#define SWITCH_HIGH    2


struct SwitchPositions
{
    uint16_t switch1;
    uint16_t switch2;
    uint16_t switch3;
};

SwitchPositions readInpSwitches()  
{
    SwitchPositions switch_positions;
    adc_reg_map *ADC1regs = ADC1->regs;

    // ADC1 is free running at max speed
    //
    // Read the switch data
    // 
    ADC1regs->SQR3 = PIN_MAP[SWITCH1].adc_channel;
    delayMicroseconds(100);
    uint16_t switch1 = (uint16_t) (ADC1regs->DR & ADC_DR_DATA);
  
    ADC1regs->SQR3 = PIN_MAP[SWITCH2].adc_channel;
    delayMicroseconds(100);
    uint16_t switch2 = (uint16_t) (ADC1regs->DR & ADC_DR_DATA);
  
    ADC1regs->SQR3 = PIN_MAP[SWITCH3].adc_channel;
    delayMicroseconds(100);
    uint16_t switch3 = (uint16_t) (ADC1regs->DR & ADC_DR_DATA);

    // Convert map readings to switch position
    //
    /*
    if(switch1 < 400)
        switch_positions.switch1 = SWITCH_LOW;
    else if(switch1 < 2000)
        switch_positions.switch1 = SWITCH_MIDDLE;
    else
        switch_positions.switch1 = SWITCH_HIGH;
  
    if(switch2 < 400)
        switch_positions.switch2 = SWITCH_LOW;
    else if(switch2 < 2000)
        switch_positions.switch2 = SWITCH_MIDDLE;
    else
        switch_positions.switch2 = SWITCH_HIGH;
  
    if(switch3 < 400)
        switch_positions.switch3 = SWITCH_LOW;
    else if(switch3 < 2000)
        switch_positions.switch3 = SWITCH_MIDDLE;
    else
        switch_positions.switch3 = SWITCH_HIGH;
    */
    switch_positions.switch1 = switch1;
    switch_positions.switch2 = switch2;
    switch_positions.switch3 = switch3;
    
    // Switch ADC1 back to capture channel
    //
    ADC1regs->SQR3 = PIN_MAP[PA0].adc_channel;
    delayMicroseconds(100);
    return switch_positions;
}

void setup()
{
    // Initialize the serial port for debugging
    // The STM32 must use Serial1
    //
    Serial1.begin(115200);
    Serial1.println("DSO-138 switch example\n\n");

    // Start 1KHz square wave on J2
    //
    pinMode(TEST_WAVE_PIN, PWM);
    Timer3.setPeriod(1000);
    pwmWrite(TEST_WAVE_PIN, 17850);
    Serial1.println("Test square wave started");
  
    // Initialize the negative voltage generator.
    // 20KHz square wave on trigger out reference and negative v gen
    //
    Timer4.setPeriod(50);   // 50 usec
    pinMode(TRIGGER_LEVEL, PWM);
    pinMode(VGEN, PWM);
    pwmWrite(VGEN, 700);
    Serial1.println("Negative voltage generator started.");
    
    // Initialize the ADC
    //
    pinMode(PA0, INPUT_ANALOG);
    adc_calibrate(ADC1);
    Serial1.println("ADC calibrated.");
    
    int pinMapADCin1 = PIN_MAP[PA0].adc_channel;
    adc_set_sample_rate(ADC1, ADC_SMPR_1_5);
    adc_set_reg_seqlen(ADC1, 1);
    ADC1->regs->SQR3 = pinMapADCin1;
    Serial1.println("ADC initialized.");
    
    // Set ADC1 to operate in continuous mode
    //
    ADC1->regs->CR2 |= ADC_CR2_CONT;
    ADC1->regs->CR1 |= 0x60000;     
    ADC1->regs->CR2 |= ADC_CR2_SWSTART;
    Serial1.println("ADCs set to operate in continuous mode.");
}

void loop()
{
    SwitchPositions switch_positions = readInpSwitches();

    Serial1.println("Switch Positions:");
    Serial1.print("  Switch 1: "); Serial1.println(switch_positions.switch1);
    Serial1.print("  Switch 2: "); Serial1.println(switch_positions.switch2);
    Serial1.print("  Switch 3: "); Serial1.println(switch_positions.switch3);
    Serial1.println("");
    delay(1000);
}
