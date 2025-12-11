#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <avl.h>
#include <wchar.h>

#define FILE_NAME "verbs.dat"

FILE *open_file(char name[])
{
    if (!name)
        return NULL;

    FILE *file = fopen(name, "rb+");

    if (!file)
        file = fopen(name, "wb+");

    return file;
}

int main()
{
    setlocale(LC_ALL, "");
    printf("\x1b[8;30;80t");
    fflush(stdout);

    FILE *file = open_file(FILE_NAME);

    Avl *portuguese_tree = new_avl();
    Avl *spanish_tree = new_avl();

    if (!portuguese_tree)
    {
        fclose(file);
        return 0;
    }

    if (!spanish_tree)
    {
        fclose(file);
        free_avl(&portuguese_tree);
        return 0;
    }

    load_from_file(file, portuguese_tree, spanish_tree);

    Verb *teste = new_verb(L"A", L"Z");

    insert_avl(portuguese_tree, teste, portuguese);
    insert_avl(spanish_tree, teste, spanish);

    save_on_file(file, portuguese_tree);

    free_data(&portuguese_tree);
    free_avl(&portuguese_tree);
    free_avl(&spanish_tree);

    fclose(file);

    return 0;
}