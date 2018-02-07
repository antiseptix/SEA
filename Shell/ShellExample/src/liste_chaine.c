/**
 * \file liste_chainee.c
 * \brief Implementation des listes chainnees
 * \author Alexis Heil, Sabrina Bourouis
 * \version 0.1
 * \date 09 fevrier 2017
 *
 * Implementation des listes chainnees
 */
#include "liste_chaine.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Fonction permettant de creer une liste vide
 *
 * \return liste renvoie une liste avec NULL comme premier element
 */
liste nouvListe() {
    liste l = (liste)malloc(sizeof(struct LISTE));
        
    l->premier = NULL;

    return l;
}

/** Ajout d'un élément en tête de la liste
 *  \param char x l'élément à ajouter
 *  \param liste l la liste
 */
void adjt(char x, liste l) {
	elt_liste enouv = (elt_liste)malloc(sizeof(struct ELT));
	
	enouv->val = x;
	enouv->suiv = l->premier;
	l->premier = enouv;
	
	return;
}
	
/** Ajout d'un élément en fin  de  liste
 *  \param char x l'élément à ajouter
 *  \param liste l la liste
 */
void adjq(char x, liste l) {
	elt_liste enouv = (elt_liste)malloc(sizeof(struct ELT));
	elt_liste cour = l->premier;
	
	// initialisation de la nouvelle cellule
	enouv->val = x;
	enouv->suiv = NULL;
	
	// si aucun element
	if (cour == NULL) { l->premier = enouv; }
	else {
		// sinon recherche du dernier
		while (cour->suiv != NULL) {
			cour = cour->suiv; 
		}
		// ajout en fin
		cour->suiv = enouv;
	}
	
	return;

}

/**
 * Fonction permettant de reccuperer le nombre d'éléments présents dans la liste 
 *
 * \param liste l pointeur sur une liste dont on souhaite recuperer la taille
 * \return int renvoie le nombre d'éléments présents dans la liste passee en parametre
 */
int taille(liste l) {
	elt_liste cour = l->premier;
	int t=0;
	while (cour) { 
		t++;
		cour = cour->suiv;
	} 
	return t;
}

/**
 * Fonction permettant de savoir si une liste est vide ou non
 *
 * \param liste l pointeur sur la liste dont on souhaite verifier si elle est vide ou pas 
 * \return int renvoie 1 si l est null, 0 sinon
 */
int estVide(liste l) {
	return (l->premier != NULL?0:1);
}

/**
 * Fonction qui rend l'element à la place i
 *
 * \param int i indice de l'element a recuperer (doit etre compris entre 0 <= i < taille(l)) 
 * \param liste l liste dans laquelle il faut recuperer l'element 
 * \return int l'element a l'indice i
 */
char elt(int i, liste l) {
    int ind = i;
    elt_liste cour = l->premier;
    
    while(ind > 0) { cour = cour->suiv; --ind; }
    
    return cour->val;
}

/**
 * Fonction permettant de liberer la liste (free)
 *
 * \param liste l liste a liberer 
 */
static void libere_liste(liste l) {
	elt_liste cour = l->premier, suiv;
	
	while (cour) {
		suiv = cour->suiv;
		free(cour);
		cour = suiv;
	}
}

/**
 * Fonction permettant de suprimer une liste
 *
 * \param liste l liste a supprimer 
 */
void fermeListe(liste l) {
	libere_liste(l);
    free(l);
}

/**
 * Fonction permettant de recuperer une liste sous forme de chaine de caractere
 *
 * \param liste l liste dont on souhaite recuperer la chaine de caractere
 * \return char* la chaine correspondant a la liste 
 */
char* getString(liste l) {
	if (l == NULL)
	{
		return NULL;
	}

	elt_liste cour = l->premier;
	char * res = malloc((taille(l)+1)*sizeof(char));
	int ind = 0;
	while(cour) {
		res[ind] = cour->val;
		ind++;
		cour = cour->suiv;
	}
	res[ind] = '\0';
	return res;
}

/**
 * Fonction permettant de comparer deux listes
 *
 * \param liste l1 liste qu'on souhaite comparer
 * \param liste l2 liste qu'on souhaite comparer
 * \return int return 1 si c'est les memes miste et -1 sinon 
 */
int cmpListe(liste l1, liste l2) {
	elt_liste cour1 = l1->premier;
	elt_liste cour2 = l2->premier;
	int same=1;
	while (cour1 && cour2 && same) { 
		if(cour1->val != cour2->val)
		{
			same = -1;
		}
		cour1 = cour1->suiv;
		cour2 = cour2->suiv;
	} 
	return same;
}
