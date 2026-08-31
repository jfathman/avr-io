// commandAdc.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Analog.h>
#include <coreError.h>
#include "commands.h"
#include "globals.h"
#include "utils.h"

static double vrefVoltage = 0.0;

static void adc_init(const char *arg2, const char *arg3);
static void adc_init_vcc(const char *arg3);
static void adc_init_int(const char *arg3);
#if defined(__AVR_ATmega2560__)
static void adc_init_int1v1(const char *arg3);
static void adc_init_int2v56(const char *arg3);
#endif 
#if defined(__AVR_ATmega4809__)
static void adc_init_int0v55(const char *arg3);
static void adc_init_int1v1(const char *arg3);
static void adc_init_int1v1(const char *arg3);
static void adc_init_int1v5(const char *arg3);
static void adc_init_int2v5(const char *arg3);
static void adc_init_int4v34(const char *arg3);
#endif 

static void adc_init_ext(const char *arg3);
static void adc_init_run(uint8_t vref);
static void adc_read(const char *arg1);

void cmd_adc(const char *args, int argc)
{
    (void) argc;

    char arg1[8] = {0};
    char arg2[8] = {0};
    char arg3[8] = {0};

    // atmega328p
    // adc init vcc|int|ext <v.v>
    // adc <pin> read

    // atmega2560
    // adc init vcc|ext <v.v>
    // adc init int1v1|int2v56 <v.v>
    // adc <pin> read

    sscanf_P(args, PSTR("%7s %7s %7s"), arg1, arg2, arg3);

    if (match_P(arg1, PSTR("init")))
    {
        adc_init(arg2, arg3);
        return;
    }

    if (match_P(arg2, PSTR("read")))
    {
        adc_read(arg1);
        return;
    }

    printf_P(PSTR("invalid args\n"));
}

static void adc_init(const char *arg2, const char *arg3)
{
    // atmega328p
    // adc init vcc|int|ext <v.v>

    // atmega2560
    // adc init vcc|ext <v.v>
    // adc init int1v1|int2v56 <v.v>

    if (strlen(arg3) > 0 && !isfloat(arg3))
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    if (match_P(arg2, PSTR("vcc")))
    {
        adc_init_vcc(arg3);
        return;
    }

    if (match_P(arg2, PSTR("int")))
    {
        adc_init_int(arg3);
        return;
    }

    #if defined(__AVR_ATmega2560__)
    if (match_P(arg2, PSTR("int1v1")))
    {
        adc_init_int1v1(arg3);
        return;
    }

    if (match_P(arg2, PSTR("int2v56")))
    {
        adc_init_int2v56(arg3);
        return;
    }
    #endif 

    #if defined(__AVR_ATmega4809__)
    if (match_P(arg2, PSTR("int0v55")))
    {
        adc_init_int0v55(arg3);
        return;
    }

    if (match_P(arg2, PSTR("int1v1")))
    {
        adc_init_int1v1(arg3);
        return;
    }

    if (match_P(arg2, PSTR("int1v5")))
    {
        adc_init_int1v5(arg3);
        return;
    }

    if (match_P(arg2, PSTR("int2v5")))
    {
        adc_init_int2v5(arg3);
        return;
    }

    if (match_P(arg2, PSTR("int4v34")))
    {
        adc_init_int4v34(arg3);
        return;
    }
    #endif 

    if (match_P(arg2, PSTR("ext")))
    {
        adc_init_ext(arg3);
        return;
    }

    printf_P(PSTR("invalid args\n"));
}

static void adc_init_vcc(const char *arg3)
{
    // <v.v> is optional for init vcc (default = 5.0v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 5.0;

    adc_init_run(VREF_VCC);
}

static void adc_init_int(const char *arg3)
{
    // <v.v> is optional for init int (default = 1.1v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 1.1;

    adc_init_run(VREF_INTERNAL);
}

#if defined(__AVR_ATmega2560__)
static void adc_init_int1v1(const char *arg3)
{
    // <v.v> is optional for init int1v1 (default = 1.1v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 1.1;

    adc_init_run(VREF_INT_1V1);
}

static void adc_init_int2v56(const char *arg3)
{
    // <v.v> is optional for init int2v56 (default = 2.56v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 2.56;

    adc_init_run(VREF_INT_2V56);
}
#endif 

#if defined(__AVR_ATmega4809__)
static void adc_init_int0v55(const char *arg3)
{
    // <v.v> is optional for init int0v55 (default = 0.55v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 0.55;

    adc_init_run(VREF_INT_0V55);
}

static void adc_init_int1v1(const char *arg3)
{
    // <v.v> is optional for init int1v1 (default = 1.1v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 1.1;

    adc_init_run(VREF_INT_1V1);
}

static void adc_init_int1v5(const char *arg3)
{
    // <v.v> is optional for init int1v5 (default = 1.5v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 1.5;

    adc_init_run(VREF_INT_1V5);
}

static void adc_init_int2v5(const char *arg3)
{
    // <v.v> is optional for init int2v5 (default = 2.5v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 2.5;

    adc_init_run(VREF_INT_2V5);
}

static void adc_init_int4v34(const char *arg3)
{
    // <v.v> is optional for init int4v34 (default = 4.34v)

    vrefVoltage = strlen(arg3) > 0 ? atof(arg3) : 4.34;

    adc_init_run(VREF_INT_4V34);
}

#endif 

static void adc_init_ext(const char *arg3)
{
    // <v.v> is not optional for init ext

    if (strlen(arg3) > 0)
    {
        vrefVoltage = atof(arg3);
        adc_init_run(VREF_EXTERNAL);
        return;
    }

    printf_P(PSTR("invalid args\n"));
}

static void adc_init_run(uint8_t vref)
{
    Analog analog;
    analog.init(vref);
    log_result();
}

static void adc_read(const char *arg1)
{
    // adc <pin> read

    if (!isdigits(arg1))
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    unsigned int pin = atoi(arg1);

    if (!isAdcPin(pin))
    {
        printf_P(PSTR("invalid pin\n"));
        return;
    }

    Analog analog;

    uint16_t reading = analog.read(pin);

    if (coreError == ERR_NONE)
    {
        double voltage = (reading / 1024.0) * vrefVoltage;

        printf_P(PSTR("%.2f\n"), voltage);
    }
    else
    {
        log_error();
    }
}
