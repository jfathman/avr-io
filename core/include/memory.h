// memory.h

#pragma once

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#if defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__AVR_ATmega4809__)

extern uint16_t _etext;
extern uint16_t _edata;
extern uint16_t __data_load_end;

extern uint16_t __data_start;
extern uint16_t __data_end;
extern uint16_t __bss_start;
extern uint16_t __bss_end;
extern uint16_t __heap_start;
extern uint16_t __stack;

extern char *__brkval;
extern char *__malloc_heap_start;
extern char *__malloc_heap_end;
extern uint16_t __malloc_margin;

typedef struct {
    size_t size;
    char data[0];
} heap_entry_t;

typedef struct __freelist {
    size_t sz;
    struct __freelist *next;
} freelist_t;

extern freelist_t *__flp;

void sramInit();
uint16_t stackUnused();
uint16_t eepromUsed();
void eepromErase();

inline uint16_t freeListSize()
{
    size_t size = 0;

    for (freelist_t *fp = __flp; fp != nullptr; fp = fp->next)
    {
        size += fp->sz + sizeof(fp->sz);
    }

    return size;
}

inline uint16_t freeListBlocks()
{
    size_t blocks = 0;

    for (freelist_t *fp = __flp; fp != nullptr; fp = fp->next)
    {
        blocks++;
    }

    return blocks;
}

inline uint16_t freeListLargestBlock()
{
    size_t maxsize = 0;

    for (freelist_t *fp = __flp; fp != nullptr; fp = fp->next)
    {
        if (fp->sz > maxsize)
        {
            maxsize = fp->sz;
        }
    }

    return maxsize;
}

inline uint16_t dataSize()
{
    return (uint16_t) &__data_end - (uint16_t) &__data_start;
}

inline uint16_t bssSize()
{
    return (uint16_t) &__bss_end - (uint16_t) &__bss_start;
}

inline uint16_t heapSize()
{
    // If __malloc_heap_end == 0 (the default), the heap size
    // varies dynamically depending on the current stack pointer
    // address.  This makes it seem as if the reported heap size
    // is unstable, but that is just because the stack pointer
    // varies so much with changes to the runtime call stack.

    // If __malloc_heap_end is set to a non-zero value to limit
    // heap memory, the reported heap size will be more stable
    // because it is not dynamic according to the stack pointer.

    // If __malloc_heap_start == 0 (the default), the heap will
    // begin at &__heap_start (same as end of .bss).  However,
    // the program can set __malloc_heap_start to relocate the
    // heap starting address.

    if (__malloc_heap_end == 0)
    {
        uint8_t entrySREG = SREG;

        cli();

        uint16_t sp = (SPH << 8) | SPL;

        SREG = entrySREG;

        return sp - __malloc_margin - (uint16_t) __malloc_heap_start;
    }

    return __malloc_heap_end - __malloc_heap_start;
}

inline uint16_t heapUsed()
{
    if (__brkval == 0)
    {
        return 0;
    }

    uint16_t used = __brkval - __malloc_heap_start;

    for (freelist_t *fp = __flp; fp != nullptr; fp = fp->next)
    {
        used -= fp->sz + sizeof(fp->sz);
    }

    return used;
}

inline uint16_t heapFree()
{
    if (heapUsed() > heapSize())
    {
        // This can occur if __malloc_heap_end == 0 which
        // allows the heap size to vary dynamically depending
        // on the current stack pointer value.  Report zero
        // instead of a negative value.
        return 0;
    }

    return heapSize() - heapUsed();
}

inline uint16_t sramStart()
{
    return RAMSTART;
}

inline uint16_t dataStart()
{
    return (uint16_t) &__data_start;
}

inline uint16_t dataEnd()
{
     return (uint16_t) &__data_end;
}

inline uint16_t bssStart()
{
     return (uint16_t) &__bss_start;
}

inline uint16_t bssEnd()
{
    return (uint16_t) &__bss_end;
}

inline uint16_t heapStart()
{
    return (uint16_t) &__heap_start;
}

inline uint16_t mallocHeapStart()
{
    return (uint16_t) __malloc_heap_start;
}

inline uint16_t brkval()
{
    return (uint16_t) __brkval;
}

inline uint16_t mallocHeapEnd()
{
    if (__malloc_heap_end == 0)
    {
        uint8_t entrySREG = SREG;

        cli();

        uint16_t sp = (SPH << 8) | SPL;

        SREG = entrySREG;

        return sp - __malloc_margin;
    }

    return (uint16_t) __malloc_heap_end;
}

inline uint16_t stackPointer()
{
    uint8_t entrySREG = SREG;

    cli();

    uint16_t sp = (SPH << 8) | SPL;

    SREG = entrySREG;

    return sp;
}

inline uint16_t stackTop()
{
    return (uint16_t) &__stack;
}

inline uint16_t stackMaximum()
{
    if (__brkval == 0)
    {
        return stackTop() - heapStart();
    }

    return stackTop() - (uint16_t) __brkval;
}

inline void stackReserve(uint16_t size)
{
    __malloc_heap_end = (char *) stackTop() - size;
}

inline uint16_t stackSize()
{
    return stackTop() - mallocHeapEnd();
}

inline uint16_t stackUsed()
{
    return stackSize() - stackUnused();
}

inline uint16_t sramEnd()
{
    return RAMEND;
}

inline uint16_t sramSize()
{
    return RAMEND - RAMSTART + 1;
}

inline uint32_t flashStart()
{
    return 0;
}

inline uint32_t flashEnd()
{
    return (uint32_t) FLASHEND;
}

inline uint32_t flashSize()
{
    return (uint32_t) FLASHEND + 1;
}

inline uint32_t flashText()
{
    // far address for flash larger than 64 KB

    return pgm_get_far_address(_etext);
}

inline uint32_t flashData()
{
    // far address for flash larger than 64 KB

    return pgm_get_far_address(__data_end) -
           pgm_get_far_address(__data_start);
}

#if defined(__AVR_ATmega4809__)
inline uint32_t flashReadOnlyData()
{
    // flash sections depend on mcu:
    //   atmega328p .text .data
    //   atmega2560 .text .data
    //   atmega4809 .text .data .rodata

    // __data_load_end includes sections above

    // far address for flash larger than 64 KB

    return pgm_get_far_address(__data_load_end)
           - (flashText() + flashData());
}
#endif

inline uint32_t flashUsed()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
    return flashText() + flashData();
    #endif

    #if defined(__AVR_ATmega4809__)
    return flashText() + flashData() + flashReadOnlyData();
    #endif
}

inline uint32_t flashFree()
{
    return flashSize() - flashUsed();
}

inline uint16_t eepromSize()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        // EEPROM starting address = 0x0000.
        return E2END + 1;
    #endif

    #if defined(__AVR_ATmega4809__)
        // EEPROM starting address = 0x1400.
        return E2END + 1 - 0x1400;
    #endif
}

inline uint16_t eepromFree()
{
    return eepromSize() - eepromUsed();
}

#endif
