#ifndef _TREE_H
#define _TREE_H

typedef struct Node Node;
struct Node
{
    int estOperateur;
    int estCommande;
    int estArgument;
    int isRoot;
    struct Node *fisDroit;
    struct Node *filsGauche;
    struct Node *nodeRoot;
    char * string; // string executanble (commande + argument) si non opératuer
};

typedef enum TypeString TypeString;
enum TypeString
{
    OPERATEUR,COMMANDE,ARGUMENT
};

/**
 * \brief Permet de detecter si la chaine de caractère contient un opérateur
 * \param char* (string) à lire
 * \return 1 si un operateur est trouvé dans la string 0 si non trouvé
 * */
int containOperator(char *str);

#endif