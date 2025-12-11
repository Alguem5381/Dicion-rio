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

Verb *new_verb_from_verb(Verb* verb)
{
    Verb *copy = (Verb*) calloc(1, sizeof(Verb));

    if (!verb)
        return NULL;

    *copy = *verb;

    return copy;
}

void delete_verb(Verb **verb)
{
    if (!verb)
        return;

    free(*verb);
    *verb = NULL;
}