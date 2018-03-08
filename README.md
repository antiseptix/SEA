# SEA

System d'Exploitation Avancé

TP et Projet Systeme d'Exploitation 2A

## Revoir toute les taboulations

	Avancement des TPs :
-TP1 "copy" : OK
     "print reverse" : OK
     " is like" : OK
	--a formater correctement pour un rendu--

-TP2 "fork" : OK
     "redirectPipe" : OK copy
     "redirectStd" : OK copy

-TP3 "compteur": OK copy
     "thread": OK

-TP4 "cgroup" : OK

-TP5 "docker" : NOK


## Lien utiles de recherche

https://openclassrooms.com/forum/sujet/couper-chaine-caractere-pour-mettre-dans-un-tableau-26376

https://www.developpez.net/forums/d1068883/general-developpement/programmation-systeme/windows/decouper-chaine-caractere-langage-c/

https://stackoverflow.com/questions/8082932/connecting-n-commands-with-pipes-in-a-shell

## TO DO 
@fixme : the hystory file who is created where the path is. (Cd recreate a file)
@TODO : déplacer la création des .o dans le répertoire bin/ et non dans src/
@TODO : copier coller les commentaires doxy dans les .h
@TODO : modification du rep include/ en header/ => à vérifier le makefile du coup !!
@TODO : doit-on commenter les structures dans les headers ? : si oui : stuc header tree.h


## Tuto Doxygen avec tonton Julien

Pour créer un espace de commentaire en début de fichier par exemple avec doxygen :

/**
 * \file nomdufichier.c  
 * \brief Courte description du fichier
 * \author Arnaud Lapenna, Julien Groll
 * \version 0.1
 * \date 23 fevrier 2018
 */

Pour commenter une fonction : 

/**
 * \brief Courte description de la fonction
 * \param Explication du premier parametre
 * \param Explication du deuxieme parametre
 * \return Qu'est ce que la fonction retourne 
 */

Et pour lancer la génération de la documentation docxygen : make doc
