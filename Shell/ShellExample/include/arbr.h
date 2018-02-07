#ifndef _ARBRE_H
#define _ARBRE_H

typedef int bool;
#define FAUX	0
#define VRAI 	1

#include <liste_chaine.h>

typedef liste T;
typedef struct NOEUD {
    T val;
    struct NOEUD *fg, *fd;
} *noeud, *arbre;

arbre nouvArbre();
arbre enracA(T racine, arbre fg, arbre fd);

arbre creerFeuilleA(T val);
T racineA(arbre);
arbre fgA(arbre);  // rend le fils gauche
arbre fdA(arbre);  // rend le fils droit
bool estVideA(arbre);    // 1 si arbre vide, 0 sinon
bool estFeuilleA(arbre); // 1 si arbre réduit à une feuille, 0 sinon

void parcoursPrefixeA(arbre a, void (*affiche)(T));

#endif
