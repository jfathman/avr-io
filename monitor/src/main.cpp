// main.cpp

#include <Mcu.h>
#include <memory.h>
#include "commands.h"
#include "globals.h"
    
int main(void)
{
    mcu.init();

    sramInit();

    stackReserve(352);

    timer.init();

    serial.init(115200);

    process_commands();
}
