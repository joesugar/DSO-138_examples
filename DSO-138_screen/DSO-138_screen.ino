#include <tuple>

#include "Screen.hpp"

// Initialization and testing the various methods.
//
void setup() 
{
    TFT::Screen screen;

    // Get the screen size.
    //
    auto x_size = screen.XSize();
    auto y_size = screen.YSize();

    // Initialize
    //
    screen.Initialize();
    screen.SetBackground(TFT::Color::RGB(0x00FF0000));  // RED
    screen.ClearScreen(); // Same as fill rectangle

    // Print a single character and string.
    //
    screen.PrintCharacter(10, 10, 0xBF);
    screen.PrintString(10, 26, "The quick brown fox");

    // Draw a couple of lines with positive and negative slope.
    //
    screen.DrawLine(0, 0, x_size - 1, y_size - 1, TFT::Color::WHITE);
    screen.DrawLine(0, y_size - 1, x_size - 1, 0, TFT::Color::WHITE);

    // Draw a rectangle, which tests drawing slopes whose abs value is >1 and <1.
    //
    screen.DrawRectangle(x_size / 4, y_size / 4, x_size / 2, y_size / 2, TFT::Color::RGB(0x000000FF));  // Blue

    // Draw some circles of different colors.
    //
    uint16_t radius = y_size / 4;
    screen.DrawCircle(x_size / 2, y_size / 2, y_size / 2, TFT::Color::WHITE);
    screen.DrawCircle((x_size / 2) - radius, (y_size / 2), radius, TFT::Color::YELLOW);
    screen.DrawCircle((x_size / 2) + radius, (y_size / 2), radius, TFT::Color::YELLOW);
    screen.DrawCircle((x_size / 2), (y_size / 2) - radius, radius, TFT::Color::YELLOW);
    screen.DrawCircle((x_size / 2), (y_size / 2) + radius, radius, TFT::Color::YELLOW);

    // Draw a filled circle.
    //
    screen.FillCircle((x_size / 2), (y_size / 2), y_size / 4, TFT::Color::GREEN);
}

// Main code
//
void loop() 
{
  
}
