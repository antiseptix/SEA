/**
 * \file arbr.c
 * \brief Implementation des arbres binaires
 * \author Alexis Heil, Sabrina Bourouis
 * \version 0.1
 * \date 09 fevrier 2017
 *
 * Implementation des arbres binaires 
 */
#include <stdio.h>
#include <stdlib.h>
#include "arbr.h"

/**
 * Fonction permettant de creer un arbre vide
 *
 * \return arbre renvoie NULL
 */
arbre nouvArbre() { return NULL; }

/**
 * Fonction permettant de creer un nouvelle arbre en ajoutant une valeur a la racine et en lui specifiant les differentes branches
 *
 * \param T racine valeur du noeud racine
 * \param arbre fg feuille gauche de l'arbre
 * \param arbre fd feuille droite de l'arbre
 * \return arbre renvoie un pointeur sur le noeud racine
 */
arbre enracA(T racine, arbre fg, arbre fd) {
	noeud nouv = (noeud)malloc(sizeof(struct NOEUD));
	
	if (!nouv) { fprintf(stderr, "malloc error"); return NULL; }
	
	nouv->val=racine;
	nouv->fg = fg; 
	nouv->fd = fd;

	return nouv;
}

/**
 * Fonction permettant de creer une feuille : un arbre avec une branche droite et une branche gauche egale a null
 *
 * \param T val valeur de la feuille a creer
 * \return arbre renvoie un pointeur sur la feuille creee
 */
arbre creerFeuilleA(T val) { return enracA(val, nouvArbre(),nouvArbre()); }

/**
 * Fonction permettant de reccuperer la valeur du noeud racine 
 *
 * \param arbre a pointeur sur la racine de l'arbre
 * \return T renvoie la valeur du noeud racine
 */
T racineA(arbre a) { 
	if(!estVideA(a)) 
	{
		return a->val;
	}
	else
	{
		return NULL;
	}
}

/**
 * Fonction permettant de reccuperer la valeur de la feuille gauche d'un arbre 
 *
 * \param arbre a pointeur sur l'arbre dont on souhaite recuperer la feuille gauche
 * \return arbre renvoie la feuille gauche de l'arbre passe en parametre
 */
arbre fgA(arbre a) { 
	if(!estVideA(a)) 
		return a->fg;
	else
		return NULL;
}

/**
 * Fonction permettant de reccuperer la valeur de la feuille droite d'un arbre 
 *
 * \param arbre a pointeur sur l'arbre dont on souhaite recuperer la feuille droite
 * \return arbre renvoie la feuille droite de l'arbre passe en parametre
 */
arbre fdA(arbre a) {
	if(!estVideA(a)) 
		return a->fd;
	else
		return NULL;
}

/**
 * Fonction permettant de savoir si un arbre est vide ou non
 *
 * \param arbre a pointeur sur l'arbre dont on souhaite verifier s'il est vide ou pas 
 * \return int renvoie 1 si a est null, 0 sinon
 */
int estVideA(arbre a) {   // 1 si arbre vide, 0 sinon
	return (a==NULL?VRAI:FAUX);
}

/**
 * Fonction permettant d'appliquer une fonction sur chacun des noeud en parcourant l'arbre de façon prefixe
 *
 * \param arbre a pointeur sur l'arbre a parcourir
 * \param void *affiche pointeur sur une fonction qui prend un parametre T
 */
void parcoursPrefixeA(arbre a, void (*affiche)(T)) {
	if (estVideA(a)) { return; }
	affiche(racineA(a));
	parcoursPrefixeA(fgA(a), affiche);
	parcoursPrefixeA(fdA(a), affiche);
}
