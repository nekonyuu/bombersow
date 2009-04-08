#ifndef LOGGING_H
#define LOGGING_H

#include <assert.h>

// Sécurisation du free()
#define free_secure(pointer) free(pointer); pointer = NULL;

void logging_Warning(char*, char*);
void logging_Error(char*, char*);
void logging_Info(char*, char*);

#endif
