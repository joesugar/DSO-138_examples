#include <tuple>

#include "Screen.hpp"

// Initialization
//
void setup() 
{
    TFT::Screen screen;

    auto x_size = screen.XSize();
    auto y_size = screen.YSize();

    screen.Initialize();

    screen.SetBackground(TFT::Color::RGB(0x00FF0000));  // RED
    screen.ClearScreen();
    
    screen.PrintCharacter(10, 10, 'A');
    screen.PrintString(10, 20, "The quick brown fox");

    uint16_t radius = y_size / 4;
     
    screen.DrawRectangle(x_size / 4, y_size / 4, x_size / 2, y_size / 2, TFT::Color::RGB(0x000000FF));  // Blue
    screen.DrawLine(0, 0, x_size - 1, y_size - 1, TFT::Color::WHITE);
    screen.DrawLine(0, y_size - 1, x_size - 1, 0, TFT::Color::WHITE);
    
    screen.DrawCircle(x_size / 2, y_size / 2, y_size / 2, TFT::Color::WHITE);
    
    screen.DrawCircle((x_size / 2) - radius, (y_size / 2), radius, TFT::Color::YELLOW);
    screen.DrawCircle((x_size / 2) + radius, (y_size / 2), radius, TFT::Color::YELLOW);
    screen.DrawCircle((x_size / 2), (y_size / 2) - radius, radius, TFT::Color::YELLOW);
    screen.DrawCircle((x_size / 2), (y_size / 2) + radius, radius, TFT::Color::YELLOW);
}

// Main code
//
void loop() 
{
  
}
