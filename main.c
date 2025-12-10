#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <avl.h>
#include <wchar.h>

int main()
{
    setlocale(LC_ALL, "");
    printf("\x1b[8;30;80t");
    fflush(stdout);

    printf("Olá mundo\n\n");

    Avl *portuguese = create_avl();

    if (!portuguese) return 0;

    show_in_order_avl(portuguese);

    insert_pt_avl(portuguese, new_verb(L"Banana", L"B"));
    insert_pt_avl(portuguese, new_verb(L"Aanana", L"A"));
    insert_pt_avl(portuguese, new_verb(L"Canana", L"A"));

    free_avl(&portuguese);

    return 0;
}