#ifndef AVL_H
#define AVL_H

#include <word.h>
#include <stdio.h>

typedef struct Avl Avl;

Avl *new_avl();

void free_avl(Avl **tree);

void free_data(Avl **tree);

int insert_avl(Avl *tree, Verb *value, Language language);

int remove_avl(Avl *tree, wchar_t word[WORD_LENGTH], Language language);

int get_size(Avl *tree);

// int avl_to_array(Avl *tree, int **array, int *array_length);

void show_in_order_avl(Avl *tree);

void show_as_tree_avl(Avl *tree);

int load_from_file(FILE *file, Avl *portuguese_tree, Avl *spanish_tree);

int save_on_file(FILE *file, Avl *tree);

#endif //AVL_H