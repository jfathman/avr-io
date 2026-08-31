// utils.cpp

#include <ctype.h>
#include <string.h>
#include "utils.h"

bool match(const char *s1, const char *s2)
{
    return strlen(s1) == strlen(s2) && strcmp(s1, s2) == 0;
}

bool match_P(const char *s1, PGM_P s2)
{
    return strlen(s1) == strlen_P(s2) && strcmp_P(s1, s2) == 0;
}

bool isdigits(const char *s)
{
    while (*s)
    {
        if (!isdigit(*s))
        {
            return false;
        }

        s++;
    }

    return true;
}

bool isfloat(const char *s)
{
    bool once = false;

    while (*s)
    {
        if (*s == '.')
        {
            if (once == true)
            {
                return false;
            }

            once = true;
        }
        else if (!isdigit(*s))
        {
            return false;
        }

        s++;
    }

    return true;
}
