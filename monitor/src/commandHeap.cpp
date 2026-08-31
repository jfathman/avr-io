// commandHeap.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

#define PRINT_VAL(text,value) \
    printf_P(PSTR("%.4u %S\n"), value, PSTR(text));

void cmd_heap(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // heap - heap usage

    PRINT_VAL("heap size",    heapSize());
    PRINT_VAL("heap used",    heapUsed());
    PRINT_VAL("heap free",    heapFree());
    PRINT_VAL("free size",    freeListSize());
    PRINT_VAL("free blocks",  freeListBlocks());
    PRINT_VAL("free largest", freeListLargestBlock());
}
