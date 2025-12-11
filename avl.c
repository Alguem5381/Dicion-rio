#include <stdlib.h>
#include <stdio.h>
#include <avl.h>
#include <wchar.h>

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Verb *value;
    int height;
} Node;

typedef struct Avl
{
    Node *root;
} Avl;

//Funções ocultas

int static set_height(Node *node)
{
    if (!node) return 0;

    int right_height = 0;
    int left_height = 0;

    if (node->left)
        left_height = node->left->height;

    if (node->right)
        right_height = node->right->height;

    if (right_height > left_height)
        node->height = right_height + 1;
    else
        node->height = left_height + 1;

    return 1;
}

int static rotate_left(Node **root)
{
    Node *node = *root;
    Node *right = node->right;

    if (!right || !node)
        return 0;

    node->right = right->left;
    right->left = node;

    set_height(node);                       //Essa ordem importa, visto que node agora é a esquerda de right
    set_height(right);                      //e right só pode ter sua altura calculada novamente
                                            //se os filho(node) estiverem com alturas corretas

    *root = right;

    return 1;
}

int static rotate_right(Node **root)
{
    Node *node = *root;
    Node *left = node->left;

    if (!left || !node)
        return 0;

    node->left = left->right;
    left->right = node;

    set_height(node);
    set_height(left);

    *root = left;

    return 1;
}

int static get_balance(Node *node)
{
    if (!node) return 0;

    int left_height = 0;
    int right_height = 0;

    if (node->left)
        left_height = node->left->height;

    if (node->right)
        right_height = node->right->height;

    return left_height - right_height;
}

int static balance(Node **pointer)
{
    Node *node = *pointer;

    if (get_balance(node) > 1)              //Se for esquerda
    {
        if (get_balance(node->left) < 0)    //Caso o filho esteja pesando para a direta
            rotate_left(&node->left);
        rotate_right(pointer);

        return 1;
    }
    else if (get_balance(node) < -1)        //Se for direita
    {
        if (get_balance(node->right) > 0)
            rotate_right(&node->right);
        rotate_left(pointer);

        return 1;
    }

    return 0;
}

int static recursive_insert_pt(Node **root, Verb *value)
{
    int result = 0;                                 //Caso o valor já existe na árvore, então os teste seguintes falharão

    if (!*root)                                  //Se o *root aponta para NULL, então insere
    {
        Node *node = (Node*)calloc(1, sizeof(Node));

        node->value = value;
        node->height = 1;

        *root = node;

        return 1;
    }

    if (wcscmp((*root)->value->portuguese, value->portuguese) > 0)                 // Se for menor, desce para a esquerda
        result = recursive_insert_pt(&(*root)->left, value);
    else if (wcscmp((*root)->value->portuguese, value->portuguese) < 0)                 // Se for maior, desce para a direita
        result = recursive_insert_pt(&(*root)->right, value);

    if (result)                                     // Atualiza a altura e rebalanceia
    {
        set_height(*root);
        balance(root);
    }

    return result;
}

int static recursive_insert_sp(Node **root, Verb *value)
{
    int result = 0;                                 //Caso o valor já existe na árvore, então os teste seguintes falharão

    if (!*root)                                  //Se o *root aponta para NULL, então insere
    {
        Node *node = (Node*)calloc(1, sizeof(Node));

        node->value = value;
        node->height = 1;

        *root = node;

        return 1;
    }

    if (wcscmp((*root)->value->spanish, value->spanish) > 0)                 // Se for menor, desce para a esquerda
        result = recursive_insert_sp(&(*root)->left, value);
    else if (wcscmp((*root)->value->spanish, value->spanish) < 0)                 // Se for maior, desce para a direita
        result = recursive_insert_sp(&(*root)->right, value);

    if (result)                                     // Atualiza a altura e rebalanceia
    {
        set_height(*root);
        balance(root);
    }

    return result;
}

Node static **find_children(Node **node)
{
    while ((*node)->right)
        node = &(*node)->right;

    return node;
}

int static recursive_remove_pt(Node **root, wchar_t *word)
{
    int result = 0;

    if (!*root)                                  //Não achou
        return result;

    if (wcscmp((*root)->value->portuguese, word) > 0)
        result = recursive_remove_pt(&(*root)->left, word);

    if (wcscmp((*root)->value->portuguese, word) < 0)
        result = recursive_remove_pt(&(*root)->right, word);

    if (!result)                                //Achou
    {
        Node *node = *root;

        if (!node->left && !node->right)        //Caso não tenha filhos
        {
            delete_verb(&(node->value));
            free(node);
            *root = NULL;
            result = 1;
        }
        else if (node->left && node->right)     //Caso dois filhos, copia o valor e repete a recursão
        {
            Node **children = find_children(&node->left);
            node->value = (*children)->value;
            result =  recursive_remove_pt(&node->left, node->value->portuguese);
        }
        else if (node->left)                    //Caso tenha ao menos um filho
        {
            *root = node->left;
            delete_verb(&(node->value));
            free(node);
            result = 1;
        }
        else
        {
            *root = node->right;
            delete_verb(&(node->value));
            free(node);
            result = 1;
        }
    }

    if (result)
    {
        set_height(*root);
        balance(root);
    }

    return result;
}

int static recursive_remove_sp(Node **root, wchar_t *word)
{
    int result = 0;

    if (!*root)                                  //Não achou
        return result;

    if (wcscmp((*root)->value->spanish, word) > 0)
        result = recursive_remove_sp(&(*root)->left, word);

    if (wcscmp((*root)->value->spanish, word) < 0)
        result = recursive_remove_sp(&(*root)->right, word);

    if (!result)                                //Achou
    {
        Node *node = *root;

        if (!node->left && !node->right)        //Caso não tenha filhos
        {
            delete_verb(&(node->value));
            free(node);
            *root = NULL;
            result = 1;
        }
        else if (node->left && node->right)     //Caso dois filhos, copia o valor e repete a recursão
        {
            Node **children = find_children(&node->left);
            node->value = (*children)->value;
            result =  recursive_remove_sp(&node->left, node->value->spanish);
        }
        else if (node->left)                    //Caso tenha ao menos um filho
        {
            *root = node->left;
            delete_verb(&(node->value));
            free(node);
            result = 1;
        }
        else
        {
            *root = node->right;
            delete_verb(&(node->value));
            free(node);
            result = 1;
        }
    }

    if (result)
    {
        set_height(*root);
        balance(root);
    }

    return result;
}

void static recursive_free(Node *root, int *delete_verb_memory)
{
    if (!root)
        return;

    recursive_free(root->left, delete_verb_memory);
    recursive_free(root->right, delete_verb_memory);

    if (*delete_verb_memory)
        delete_verb(&(root->value));
    else
        free(root);
}

void static recursive_get_size(Node *root, int *count)
{
    if (!root)
        return;

    recursive_get_size(root->left, count);
    recursive_get_size(root->right, count);

    count++;
}

/*int static recursive_to_array(Node *root, int **array, int *array_length, int *count)
{
    if (!root)
        return 0;

    if (recursive_to_array(root->left, array, array_length, count))    //Desce na recursão

    if (array_length && *count >= *array_length)                      //Aumenta o vetor caso tenha chegado no limite
    {
        *array_length *= 2;
        int *temp = (int*)realloc(*array, sizeof(int) * *array_length);

        if (!temp)
        {
            free(*array);
            *array = NULL;
            *array_length = 0;
            return 0;
        }

        *array = temp;
    }

    if (array_length)
        (*array)[(*count)++] = root->value;                  //Coloca o valor no vetor

    if (recursive_to_array(root->right, array, array_length, count))

    return 1;
}*/

void static recursive_show_tree(Node *root, int level)
{
    if (!root)
        return;

    recursive_show_tree(root->right, level + 1);
    printf("%*ls\n", level * 2, root->value->portuguese);
    recursive_show_tree(root->left, level + 1);
}

void static recursive_show_in_order(Node *root)
{
    if (!root)
        return;

    recursive_show_in_order(root->left);
    printf("%ls ", root->value->portuguese);
    recursive_show_in_order(root->right);
}

void static recursive_save_on_file(FILE *file, Node *node)
{
    if (!file || !node)
        return;

    fwrite(node->value, sizeof(Verb), 1, file);
    recursive_save_on_file(file, node->left);
    recursive_save_on_file(file, node->right);
}

//Funções expostas:

Avl *new_avl()
{
    return (Avl*)calloc(1, sizeof(Avl));
}

void free_data(Avl **tree)
{
    if (!*tree) return;

    int delete = 1;
    recursive_free((*tree)->root, &delete);
}

void free_avl(Avl **tree)
{
    if (!*tree) return;

    int delete = 0;
    recursive_free((*tree)->root, &delete);
    free(*tree);

    *tree = NULL;
}

int insert_avl(Avl *tree, Verb *value, Language language)
{
    if (!tree || !value)
        return 0;

    if (language == portuguese)
        return recursive_insert_pt(&tree->root, value);
    else if (language == spanish)
        return recursive_insert_sp(&tree->root, value);
}

int remove_avl(Avl *tree, wchar_t word[WORD_LENGTH], Language language)
{
    if (!tree)
        return 0;

    if (language == portuguese)
        return recursive_remove_pt(&tree->root, word);
    else if (language == spanish)
        return recursive_remove_sp(&tree->root, word); 
}



int get_size(Avl *tree)
{
    if (!tree)
        return 0;

    int count = 0;

    recursive_get_size(tree->root, &count);

    return count;
}

/*int avl_to_array(Avl *tree, int **array, int *array_length)
{
    if (!tree || *array || !array_length)
        return 0;

    int count = 0;
    *array_length = 1;
    *array = (int*)calloc(*array_length, sizeof(int));
    recursive_to_array(tree->root, array, array_length, &count);

    *array_length = count;
}*/

void show_in_order_avl(Avl *tree)
{
    if (!tree)
        return;

    recursive_show_in_order(tree->root);
}

void show_as_tree_avl(Avl *tree)
{
    if (!tree)
        return;

    recursive_show_tree(tree->root, 1);
    printf("\n");
}

int load_from_file(FILE *file, Avl *portuguese_tree, Avl *spanish_tree)
{
    if (!file || !portuguese_tree)
        return 0;

    Verb verb = {0};

    while (fread(&verb, sizeof(Verb), 1, file) == 1)
    {
        Verb *copy = new_verb_from_verb(&verb);

        if (!copy)
            return 0;

        insert_avl(portuguese_tree, copy, portuguese);
        insert_avl(spanish_tree, copy, spanish);
    }

    fseek(file, 0, SEEK_SET);

    return 1;
}

int save_on_file(FILE *file, Avl *tree)
{
    if (!file || !tree)
        return 0;

    recursive_save_on_file(file, tree->root);

    fseek(file, 0, SEEK_SET);

    return 1;
}