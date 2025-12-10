#ifndef WORD_H
#define WORD_H

#define WORD_LENGTH 64

#include <wchar.h>

typedef struct Verb
{
    wchar_t portuguese[WORD_LENGTH];
    wchar_t spanish[WORD_LENGTH];
} Verb;

Verb *new_verb(wchar_t *portuguese, wchar_t *spanish);

void delete_verb(Verb **verb);

#endif //WORD_H