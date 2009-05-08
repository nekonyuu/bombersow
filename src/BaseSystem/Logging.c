#include <stdio.h>
#include <stdlib.h>
#include "BaseSystem/Logging.h"

void logging_Warning(char* fct_name, char* text)
{
    printf("Warning - %s : %s\n", fct_name, text);
}

void logging_Error(char* fct_name, char* text)
{
    printf("Error - %s : %s\n", fct_name, text);
    exit(1);
}

void logging_Info(char* fct_name, char* text)
{
#ifdef DEBUG_MODE
    printf("Debug Info - %s : %s\n", fct_name, text);
#endif
}
