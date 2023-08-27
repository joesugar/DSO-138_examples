#ifndef COMMANDS_HPP
#define COMMANDS_HPP

namespace TFT
{
    // Various commands of use for the ILI9341 display
    //
    const uint8_t DISPLAY_OFF     = 0x28;
    const uint8_t DISPLAY_ON      = 0x29;
    const uint8_t SET_COLUMN      = 0x2A;
    const uint8_t SET_ROW         = 0x2B;
    const uint8_t WRITE_TO_MEMORY = 0x2C;
}

#endif
