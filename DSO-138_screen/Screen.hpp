#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Font.hpp"

#define TFT_Port (GPIOB_BASE->ODR)

namespace TFT
{
    // Class to control the ILI9341 display used as part 
    // of the DSO-138.
    //
    class Screen
    {
      public:
        Screen(void);
        ~Screen(void);

        void Initialize(void);
        
        void SetForeground(uint16_t color);
        void SetBackground(uint16_t color);
        
        void ClearScreen(void);
        void ClearScreen(uint16_t color);

        void DrawPixel(uint16_t x1, uint16_t y1, uint16_t color);
        void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
        
        void DrawRectangle(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color);
        void FillRectangle(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color);
        
        void DrawCircle(uint16_t xc, uint16_t yc, uint16_t radius, uint16_t color);
        void FillCircle(uint16_t xc, uint16_t yc, uint16_t radius, uint16_t color);
        
        void PrintCharacter(uint16_t x, uint16_t y, uint8_t ch);
        void PrintString(uint16_t x, uint16_t y, char* string);

        const uint16_t XSize() { return screenXSize_; };
        const uint16_t YSize() { return screenYSize_; };
        
      private:
        void write_data(uint8_t);
        void write_comm(uint8_t);
        void init_io(void);
        void init_screen(void);
        void set_window(uint16_t x, uint16_t xsize, uint16_t y, uint16_t ysize);
        void display_partial_circle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color);
        void fill_partial_circle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color);
        void set_pixel_location(uint16_t x, uint16_t y);
        void set_pixel_color(uint16_t color);
        void draw_line_slope_lt_one(
              uint16_t x0, uint16_t y0, int16_t dx, int16_t dy, int16_t sx, int16_t sy, uint16_t color);
        void draw_line_slope_ge_one(
              uint16_t x0, uint16_t y0, int16_t dx, int16_t dy, int16_t sx, int16_t sy, uint16_t color);
        
        uint16_t foreground_;
        uint16_t background_;
        FONT*    font_;
        
        const uint16_t screenXSize_;
        const uint16_t screenYSize_;

        const bool HI = true;
        const bool LO = false;
    };


    #define RGB_(R16, G16, B16)   (((R16 >> 3) << 0) | ((G16 >> 2) << 5) | ((B16 >> 3) << 11))
    class Color
    {
      public:
        static uint16_t RGB(uint8_t R, uint8_t G, uint8_t B);
        static uint16_t RGB(uint32_t hex_color_code);
        
        static const uint16_t BLACK   = RGB_(  0,   0,   0);
        static const uint16_t WHITE   = RGB_(255, 255, 255);
        static const uint16_t RED     = RGB_(255,   0,   0);
        static const uint16_t GREEN   = RGB_(  0, 255,   0);
        static const uint16_t BLUE    = RGB_(  0,   0, 255);
        static const uint16_t YELLOW  = RGB_(255, 255,   0);
        static const uint16_t CYAN    = RGB_(  0, 255, 255);
        static const uint16_t MAGNETA = RGB_(255,   0, 255);
    };
}

#endif
