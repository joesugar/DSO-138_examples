#include "Screen.hpp"

// Initialization
//
void setup() 
{
    TFT::Screen screen;
    screen.Initialize();

    screen.SetBackground(TFT::Color::RGB(255, 0, 0));
    screen.ClearScreen();
    
    screen.PrintCharacter(10, 10, 'A');
    screen.PrintString(10, 20, "The quick brown fox");

    screen.DrawLine(0, 0, 319, 239, TFT::Color::RGB(255, 255, 255));
    screen.DrawLine(0, 239, 319, 0, TFT::Color::RGB(255, 255, 255));
    
    screen.DrawCircle(160, 120, 120, TFT::Color::RGB(255, 255, 255));
    screen.DrawCircle( 80, 120,  60, TFT::Color::RGB(255, 255, 255));
    screen.DrawCircle(240, 120,  60, TFT::Color::RGB(255, 255, 255));
    screen.DrawCircle(160,  60,  60, TFT::Color::RGB(255, 255, 255));
    screen.DrawCircle(160, 180,  60, TFT::Color::RGB(255, 255, 255));

}

// Main code
//
void loop() 
{
  
}
