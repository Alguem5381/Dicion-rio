#include <word.h>
#include <stdlib.h>

Verb *new_verb(wchar_t *portuguese, wchar_t *spanish)
{
    Verb *verb = (Verb*) calloc(1, sizeof(Verb));

    if (!verb)
        return NULL;

    wcsncpy(verb->portuguese, portuguese, WORD_LENGTH);
    wcsncpy(verb->spanish, spanish, WORD_LENGTH);

    return verb;
}

void delete_verb(Verb **verb)
{
    if (!verb)
        return;

    free(*verb);
    *verb = NULL;
}