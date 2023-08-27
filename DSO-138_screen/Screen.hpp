#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Font.hpp"

#define SetToLow(port, bit_mask)    (port->BRR = bit_mask)
#define SetToHigh(port, bit_mask) (port->BSRR = bit_mask)

#define TFT_Port            (GPIOB_BASE->ODR)
#define TFT_nRESET_Port     GPIOB_BASE
#define TFT_nRESET_Bit      11    
#define TFT_RS_Port         GPIOC_BASE
#define TFT_RS_Bit          14  
#define TFT_nCS_Port        GPIOC_BASE
#define TFT_nCS_Bit         13  
#define TFT_nWR_Port        GPIOC_BASE
#define TFT_nWR_Bit         15  
#define TFT_nRD_Port        GPIOB_BASE
#define TFT_nRD_Bit         10  

namespace TFT
{
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
        
        void FillRectangle(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color);
        void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
        
        void PrintCharacter(uint16_t x, uint16_t y, uint8_t ch);
        void PrintString(uint16_t x, uint16_t y, char* string);

      private:
        void write_data(uint8_t);
        void write_comm(uint8_t);
        void init_io(void);
        void init_screen(void);
        void set_window(uint16_t x, uint16_t xsize, uint16_t y, uint16_t ysize);
        void set_pixel_location(uint16_t x, uint16_t y);

        uint16_t foreground_;
        uint16_t background_;
        uint16_t screenXSize_;
        uint16_t screenYSize_;
        FONT*    font_;
        
        const bool HI = true;
        const bool LO = false;
    };

    class Color
    {
      public:
        static uint16_t RGB(uint8_t R, uint8_t G, uint8_t B);
    };

    uint8_t DISPLAY_OFF     = 0x28;
    uint8_t DISPLAY_ON      = 0x29;
    uint8_t SET_COLUMN      = 0x2A;
    uint8_t SET_ROW         = 0x2B;
    uint8_t WRITE_TO_MEMORY = 0x2C;
}

#endif
