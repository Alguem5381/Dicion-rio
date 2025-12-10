#ifndef AVL_H
#define AVL_H

#include <word.h>

typedef struct Avl Avl;

Avl *create_avl();

void free_avl(Avl **tree);

int insert_pt_avl(Avl *tree, Verb *value);

int insert_sp_avl(Avl *tree, Verb *value);

int remove_pt_avl(Avl *tree, wchar_t word[WORD_LENGTH]);

int remove_sp_avl(Avl *tree, wchar_t word[WORD_LENGTH]);

int get_size(Avl *tree);

// int avl_to_array(Avl *tree, int **array, int *array_length);

void show_in_order_avl(Avl *tree);

void show_as_tree_avl(Avl *tree);

#endif //AVL_H