
// builds the tree
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int initSampleTree(Node *node)
{
    printf("Initinalisation de l'arbre de test correspondant à 'ls -l && cd .. || echo banane'\n");


    return 0;
}
/**
 * \brief Permet de detecter si la chaine de caractère contient un opérateur
 * \param char* (string) à lire
 * \return 1 si un operateur est trouvé dans la string 0 si non trouvé
 * */
int containOperator(char *str)
{
    char * result;
    result = strpbrk(str, "&|;<>");
    if (result!=NULL){
        return 1;
    }
    return 0;
}

/*
struct node {
    int data;
    struct node *left;
    struct node *right;
};

int fun7(struct node *n, int value) {
    if (n == NULL) {
        return -1;
    }

    if (n->data <= value) {
        if (n->data == value) {
            return 0;
        }
        return 2 * fun7(n->right, value) + 1;
    } else {
        return 2 * fun7(n->left, value);
    }
}

void print_tree(struct node *root, int x) {
    if (root == NULL) {
        return;
    }

    printf("%#.3x\n", root->data);

    if (root->left != NULL) {
        for (int i = 0; i < x; i++) {
            printf(" ");
        }
    }

    print_tree(root->left, x * 2);

    if (root->right != NULL) {
        for (int i = 0; i < x; i++) {
            printf(" ");
        }
    }
    print_tree(root->right, x * 2);
}

void insert(struct node *root, struct node *node) {
    if (root->data > node->data) {
        if (root->left == NULL) {
            root->left = node;
        } else {
            insert(root->left, node);
        }
    } else {
        if (root->right == NULL) {
            root->right = node;
        } else {
            insert(root->right, node);
        }
    }
}

int main() {
    struct node *root = malloc(sizeof(struct node));
    root->data = 0x24;
    root->left = NULL;
    root->right = NULL;

    int values[] = {0x8, 0x32, 0x6, 0x16, 0x2d, 0x6b, 0x1, 0x7, 0x14, 0x23, 0x28, 0x2f, 0x63, 0x3e9};

    for (int i = 0; i < (int) (sizeof(values)/sizeof(int)); i++) {
        struct node *n = malloc(sizeof(struct node));
        n->data = values[i];
        n->left = NULL;
        n->right = NULL;
        insert(root, n);
    }

    print_tree(root, 2);

    for (int i = 0; i < 1000; i++) {
        if (fun7(root, i) == 2) {
            printf("answer: %d\n", i);
        }
    }
}
*/