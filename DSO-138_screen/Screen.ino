#include <stdint.h>

#include "Screen.hpp"
#include "Font.hpp"
#include "Commands.hpp"

namespace TFT
{
    #define TFT_nRESET_Bit      11    // Port GPIOB
    #define TFT_RS_Bit          14    // Port GPIOC
    #define TFT_nCS_Bit         13    // Port GPIOC
    #define TFT_nWR_Bit         15    // Port GPIOC
    #define TFT_nRD_Bit         10    // Port GPIOB

    // Constructor.
    //
    Screen::Screen() :
        foreground_(Color::RGB(255, 255, 255)),
        background_(Color::RGB(0, 0, 0)),
        font_(&ASC8X16),
        screenXSize_(320),
        screenYSize_(240)
    {
        
    }

    
    // Destructor
    //
    Screen::~Screen()
    {
      
    }


    // Initialize the screen.
    //
    void Screen::Initialize()
    {
        init_io();
        init_screen();
    }


    // Set the foreground color
    //
    void Screen::SetForeground(uint16_t color)
    {
        foreground_ = color;  
    }


    // Set the background color
    //
    void Screen::SetBackground(uint16_t color)
    {
        background_ = color;
    }

    
    // Clear the screen
    ///
    void Screen::ClearScreen(void)
    {
        FillRectangle(0, 0, 320, 240, background_);
    }


    // Clear the screen with a particular color.
    //
    void Screen::ClearScreen(uint16_t color)
    {
        FillRectangle(0, 0, screenXSize_, screenYSize_, color);
    }


    // Draw an individual pixel.
    //
    void Screen::DrawPixel(uint16_t x0, uint16_t y0, uint16_t color)
    {
        set_pixel_location(x0, y0);
        set_pixel_color(color);
        return;
    }

    
    // Draw a line on the screen.
    // Algorithm from https://iq.opengenus.org/bresenham-line-drawining-algorithm/
    //
    void Screen::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
    {
        // Switch points so (x0, y0) is on the left hand side
        //
        if (x0 > x1)
        {
            uint16_t tmpx = x0;
            uint16_t tmpy = y0;
            x0 = x1;
            y0 = y1;
            x1 = tmpx;
            y1 = tmpy;
        }

        // Initialize required values.
        //
        int16_t dx = (x1 > x0) ? x1 - x0 : x0 - x1;    // abs(x1 - x0)
        int16_t dy = (y1 > y0) ? y1 - y0 : y0 - y1;    // abs(y1 - y0)
        int16_t sx = (x1 > x0) ? 1 : -1;               // sgn(x1 - x0)
        int16_t sy = (y1 > y0) ? 1 : -1;               // sgn(y1 - y0)

        if (dx > dy)
        {
            draw_line_slope_lt_one(x0, y0, dx, dy, sx, sy, color);
        }
        else
        {
            draw_line_slope_ge_one(x0, y0, dx, dy, sx, sy, color);
        }
        return;
    }


    // Draw a circle on the display with the given color.
    // Algorithm from https://iq.opengenus.org/bresenhams-circle-drawing-algorithm/
    //
    void Screen::DrawCircle(uint16_t xc, uint16_t yc, uint16_t radius, uint16_t color)
    {
        uint16_t x = 0;
        uint16_t y = radius;
        
        int16_t decision_parameter = 3 - 2 * radius;
        display_partial_circle(xc, yc, x, y, color);
        while (y >= x)
        {
            x++;
            if (decision_parameter > 0)
            {
                y--;
                decision_parameter = decision_parameter + 4 * (x - y) + 10;
            }
            else
            {
                decision_parameter = decision_parameter + 4 * x + 6;
            }
            display_partial_circle(xc, yc, x, y, color);
        }
        return;
    }


    // Fill a circle on the display with the given color.
    // Algorithm from https://iq.opengenus.org/bresenhams-circle-drawing-algorithm/
    //
    void Screen::FillCircle(uint16_t xc, uint16_t yc, uint16_t radius, uint16_t color)
    {
        uint16_t x = 0;
        uint16_t y = radius;
        
        int16_t decision_parameter = 3 - 2 * radius;
        fill_partial_circle(xc, yc, x, y, color);
        while (y >= x)
        {
            x++;
            if (decision_parameter > 0)
            {
                y--;
                decision_parameter = decision_parameter + 4 * (x - y) + 10;
            }
            else
            {
                decision_parameter = decision_parameter + 4 * x + 6;
            }
            fill_partial_circle(xc, yc, x, y, color);
        }
        return;
    }


    // Draw a rectange on the screen.
    //
    void Screen::DrawRectangle(int16_t x, int16_t y, int16_t x_size, int16_t y_size, uint16_t color)
    {
        DrawLine(x, y, x + x_size - 1, y, color);
        DrawLine(x, y, x, y + y_size - 1, color);
        DrawLine(x + x_size - 1, y + y_size - 1, x + x_size - 1, y, color);
        DrawLine(x + x_size - 1, y + y_size - 1, x, y + y_size - 1, color);
        return;  
    }
    
    // Fill a rectangle defined on the screen.
    //
    void Screen::FillRectangle(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color)
    {
        uint32_t tmp; 
    
        set_window(x, xsize, y, ysize);

        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
        gpio_write_bit(GPIOC, TFT_RS_Bit, LO);
    
        TFT_Port = (TFT_Port & 0xFF00) | WRITE_TO_MEMORY;
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);
    
        tmp = static_cast<uint32_t>(xsize) * static_cast<uint32_t>(ysize);
        
        // Set up to access Index Register (RS == 1)
        gpio_write_bit(GPIOC, TFT_RS_Bit, HI);
     
        while(tmp) 
        {
            TFT_Port = (TFT_Port & 0xFF00) | (color >> 8);
            gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
            gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

            TFT_Port = (TFT_Port & 0xFF00) | color;
            gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
            gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);
      
            tmp--;
        }
    
        // Set TFT_nCS high
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
    }


    // Write a character to the given location 
    //
    void Screen::PrintCharacter(uint16_t x, uint16_t y, uint8_t ch)
    {
        uint8_t  char_mask;
        uint16_t char_byte_count;
        uint16_t pixel_color;
        uint8_t  x_size;
        uint8_t  *char_mask_index;

        // No control characters.
        //
        if (ch < 0x0020)
            return;
            
        // Calculate the index into the font array.
        //
        char_mask_index = (uint8_t *)font_->Array + ch * ((font_->Xsize + 7)/8) * font_->Ysize;

        // Writing to a limited portion of the screen.
        //
        set_window(x, font_->Xsize, y, font_->Ysize);

        // Command to write to graphics memory.
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
        gpio_write_bit(GPIOC, TFT_RS_Bit, LO);

        TFT_Port = (TFT_Port & 0xFF00) | WRITE_TO_MEMORY;
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

        gpio_write_bit(GPIOC, TFT_RS_Bit, HI);

        char_byte_count = (font_->Xsize * font_->Ysize)/8;
        while(char_byte_count > 0) 
        {
            char_mask = *char_mask_index;
            x_size = 8;
            while(x_size > 0) 
            {
                pixel_color = (char_mask & 0x80) ? foreground_ : background_;

                TFT_Port = (TFT_Port & 0xFF00) | (pixel_color >> 8);
                gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
                gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);
                
                TFT_Port = (TFT_Port & 0xFF00) | (pixel_color >> 0);
                gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
                gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

                char_mask <<= 1;
                x_size--;
            }
            char_mask_index++;
            char_byte_count--; 
        }

        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
    }


    // Write a string to the given location
    //
    void Screen::PrintString(uint16_t x, uint16_t y, char *str)
    {
        uint8_t tmp;
    
        while ((tmp = *str++)) 
        {
            PrintCharacter(x, y, tmp);
            x += font_->CharPitch;
            if (x >= screenXSize_) 
            {
                y += font_->LinePitch;
                x = 0;
            }
        }
    }


    // Initialize the pins used to communicate with the
    // screen
    //
    void Screen::init_io()
    {
        // Parallel port to the screen.
        //
        gpio_set_mode(GPIOB, 0, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 2, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 3, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 4, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 5, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 6, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, 7, GPIO_OUTPUT_PP);

        // Pins used to control the screen.
        //
        gpio_set_mode(GPIOB, TFT_nRESET_Bit, GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOC, TFT_RS_Bit,     GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOC, TFT_nCS_Bit,    GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOC, TFT_nWR_Bit,    GPIO_OUTPUT_PP);
        gpio_set_mode(GPIOB, TFT_nRD_Bit,    GPIO_OUTPUT_PP);
    }
    
    // Initialize the screen object.
    //
    void Screen::init_screen()
    {
        uint8_t  tmp;

        // Reset TFT controller (Ili9341)
        //
        gpio_write_bit(GPIOB, TFT_nRESET_Bit, HI);
        delay(10);  
        gpio_write_bit(GPIOB, TFT_nRESET_Bit, LO);
        delay(20); 
        gpio_write_bit(GPIOB, TFT_nRESET_Bit, HI);
        delay(20);

        // CS HIGH, WR HIGH, RD HIGH, CS LOW
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, HI);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);
        gpio_write_bit(GPIOB, TFT_nRD_Bit, HI);
  
        tmp = 10;
        while(tmp) 
        {
            delay(15);
            tmp--;
        }
 
        write_comm(0xcf);   // Power control B
        write_data(0x00);
        write_data(0xC1);
        write_data(0x30);

        write_comm(0xed);   // Power on sequence control
        write_data(0x67);
        write_data(0x03);
        write_data(0x12);
        write_data(0x81);

        write_comm(0xcb);   // Power control A
        write_data(0x39);
        write_data(0x2c);
        write_data(0x00);
        write_data(0x34);
        write_data(0x02);

        write_comm(0xea);   // Driver timing control B
        write_data(0x00);
        write_data(0x00);
      
        write_comm(0xe8);   // Driver timing control A
        write_data(0x85);
        write_data(0x0a);
        write_data(0x78);
      
        write_comm(0xF7);   // Pump ratio control
        write_data(0x20);

        write_comm(0xC0);   // Power control
        write_data(0x26);   // VRH[5:0]
      
        write_comm(0xC1);   // Power control
        write_data(0x01);   // SAP[2:0];BT[3:0]
      
        write_comm(0xC5);   // VCOM control 1
        write_data(0x2b);
        write_data(0x2F);

        write_comm(0xc7);   // VCOM Control 2(
        write_data(0xc7);
      
        write_comm(0x3A);   // Set pixel format
        write_data(0x55);
      
        write_comm(0x36);   // Memory Access Control
        write_data(0x20);
        
        write_comm(0xB1);   // Frame Rate Control
        write_data(0x00);
        write_data(0x18);
        
        write_comm(0xB6);   // Display Function Control
        write_data(0x0a);
        write_data(0xE2);
  
        write_comm(0xF2);   // 3Gamma Function Disable
        write_data(0x00);
        
        write_comm(0x26);   // Gamma curve selected
        write_data(0x01);
        
        write_comm(0xE0);   // Set Gamma
        write_data(0x0f);
        write_data(0x1d);
        write_data(0x1a);
        write_data(0x09);
        write_data(0x0f);
        write_data(0x09);
        write_data(0x46);
        write_data(0x88);
        write_data(0x39);
        write_data(0x05);
        write_data(0x0f);
        write_data(0x03);
        write_data(0x07);
        write_data(0x05);
        write_data(0x00);

        write_comm(0XE1);   // Set Gamma
        write_data(0x00);
        write_data(0x22);
        write_data(0x25);
        write_data(0x06);
        write_data(0x10);
        write_data(0x06);
        write_data(0x39);
        write_data(0x22);
        write_data(0x4a);
        write_data(0x0a);
        write_data(0x10);
        write_data(0x0c);
        write_data(0x38);
        write_data(0x3a);
        write_data(0x0F);

        write_comm(0x11);   // Exit Sleep
        tmp = 100;
        while(tmp) 
        {
            delay(12);
            tmp--;
        }
        
        write_comm(DISPLAY_ON); //display on  

        delay(12);
        delay(12);
    }

    void Screen::write_comm(uint8_t commport)
    {
        // Set TFT_nCS low
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
 
        // Set up to access command register (RS == 0)
        //
        gpio_write_bit(GPIOC, TFT_RS_Bit, LO);

        // Write out the value.
        //
        TFT_Port = (TFT_Port & 0xFF00) | commport;
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

        // Set up to access Data Register (RS == 1)
        //
        gpio_write_bit(GPIOC, TFT_RS_Bit, HI);

        // Set TFT_nCS high
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, HI);
    }

    void Screen::write_data(uint8_t data)
    {
        // Set TFT_nCS low
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
      
        // Set up to access data register (RS == 1)
        //
        gpio_write_bit(GPIOC, TFT_RS_Bit, HI);

        // Write out the value.
        //
        TFT_Port = (TFT_Port & 0xFF00) | data;
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);
      
        // Set TFT_nCS high
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, HI);
    }


    // Move to a specific screen location.
    //
    void Screen::set_pixel_location(uint16_t x, uint16_t y)
    {
        set_window(x, 1, y, 1);
        return;
    }

    
    // Set a logical windows on the screen.
    //
    void  Screen::set_window(uint16_t x, uint16_t xsize, uint16_t y, uint16_t ysize)
    {
         write_comm(SET_COLUMN);
        
         write_data(x >> 8);
         write_data(x);
         
         x = x + xsize - 1;
         write_data(x >> 8);
         write_data(x);
        
         write_comm(SET_ROW);
         
         write_data(y >> 8);
         write_data(y);
         
         y = y + ysize - 1;
         write_data(y >> 8);
         write_data(y);
    }


    // Set the pixel at the current location to
    // the specified color.
    //
    void Screen::set_pixel_color(uint16_t color)
    {
        // Draw the pixel
        //
        // Select the display.
        // 
        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);

        // Write command to memory
        //
        gpio_write_bit(GPIOC, TFT_RS_Bit, LO);    // Command register
        
        TFT_Port = (TFT_Port & 0xFF00) | WRITE_TO_MEMORY;
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

        // Write data to memory
        //
        gpio_write_bit(GPIOC, TFT_RS_Bit, HI);    // Data register

        TFT_Port = (TFT_Port & 0xFF00) | (color >> 8);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

        TFT_Port = (TFT_Port & 0xFF00) | color;
        gpio_write_bit(GPIOC, TFT_nWR_Bit, LO);
        gpio_write_bit(GPIOC, TFT_nWR_Bit, HI);

        // Unselect the display.
        //
        gpio_write_bit(GPIOC, TFT_nCS_Bit, LO);
    }


    // Draw a line with slope whose absolute value < 1.
    //
    void Screen::draw_line_slope_lt_one(
        uint16_t x0, uint16_t y0, int16_t dx, int16_t dy, int16_t sx, int16_t sy, uint16_t color)
    {
        int16_t pk = (2 * dy) - dx;
        for (int16_t i = 0, x = x0, y = y0; i < dx ; i++, x += sx)
        {
            // Set the pixel.
            //
            set_pixel_location(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
            set_pixel_color(color);
                      
            // Update the pixel locations.
            //
            if (pk < 0)
            {
                pk += 2 * dy;
            }
            else
            {
                y += sy;
                pk += (2 * (dy - dx));
            }
        }
    }


    // Draw a line with slope whose absolute value >= 1.
    //
    void Screen::draw_line_slope_ge_one(
        uint16_t x0, uint16_t y0, int16_t dx, int16_t dy, int16_t sx, int16_t sy, uint16_t color)
    {
        int16_t pk = (2 * dx) - dy;
        for (int16_t i = 0, x = x0, y = y0; i < dy ; i++, y += sy)
        {
            // Set the pixel.
            //
            set_pixel_location(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
            set_pixel_color(color);
                      
            // Update the pixel locations.
            //
            if (pk < 0)
            {
                pk += 2 * dx;
            }
            else
            {
                x += sx;
                pk += (2 * (dx - dy));
            }
        }
    }
    

    // Draw points in each of the 8 octets of rhe circle.
    //
    void Screen::display_partial_circle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color) 
    {
        // Displaying all 8 coordinates of(x,y) residing in 8-octants
        set_pixel_location(xc + x, yc + y);
        set_pixel_color(color);
        
        set_pixel_location(xc - x, yc + y);
        set_pixel_color(color);
        
        set_pixel_location(xc + x, yc - y);
        set_pixel_color(color);
        
        set_pixel_location(xc - x, yc - y);
        set_pixel_color(color);
        
        set_pixel_location(xc + y, yc + x);
        set_pixel_color(color);
        
        set_pixel_location(xc - y, yc + x);
        set_pixel_color(color);
        
        set_pixel_location(xc + y, yc - x);
        set_pixel_color(color);
        
        set_pixel_location(xc - y, yc - x);
        set_pixel_color(color);
    }


    // Draw a line connecting points in each of the 8 octets of the circle.
    //
    void Screen::fill_partial_circle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color) 
    {
        draw_line_slope_lt_one(xc - x, yc + y, 2 * x, 0, 1, 1, color);
        draw_line_slope_lt_one(xc - x, yc - y, 2 * x, 0, 1, 1, color);
        draw_line_slope_lt_one(xc - y, yc + x, 2 * y, 0, 1, 1, color);
        draw_line_slope_lt_one(xc - y, yc - x, 2 * y, 0, 1, 1, color);
        return; 
    }

    // Create a color from the given components.
    //
    uint16_t Color::RGB(uint8_t R, uint8_t G, uint8_t B)
    {
        uint16_t R16 = static_cast<uint16_t>(R);
        uint16_t G16 = static_cast<uint16_t>(G);
        uint16_t B16 = static_cast<uint16_t>(B);
        return RGB_(R16, G16, B16);
    }

    // Create a color from a given hex color code.
    // The code is of the form 0x00RRGGBB
    //
    uint16_t Color::RGB(uint32_t color_code)
    {
        uint8_t R16 = static_cast<uint16_t>((color_code >> 16) & 0x000000FF);
        uint8_t G16 = static_cast<uint16_t>((color_code >>  8) & 0x000000FF);
        uint8_t B16 = static_cast<uint16_t>((color_code >>  0) & 0x000000FF);
        return RGB_(R16, G16, B16);
    }
}
