// utils.h

#pragma once

#include <avr/pgmspace.h>

#define ROWS(arr) ((sizeof(arr)) / (sizeof(arr[0])))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define STR_HELPER(x) #x
#define TO_STR(x) STR_HELPER(x)

bool match(const char *s1, const char *s2);
bool match_P(const char *s1, PGM_P s2);
bool isdigits(const char *s);
bool isfloat(const char *s);
