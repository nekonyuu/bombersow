#ifndef LOGGING_H
#define LOGGING_H

#include <assert.h>

// S�curisation du free() (Evite les crash sur un free d�j� effectu�)
#define free_secure(pointer) free(pointer); pointer = NULL

void logging_Warning(char*, char*);
void logging_Error(char*, char*);
void logging_Info(char*, char*);

#endif
