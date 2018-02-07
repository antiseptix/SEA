/**
 * \file StringGoodies.c
 * \brief Fichier utilitaire créer pour faciliter le traitement des chaînes de caractères
 * \author Alexis Heil, Sabrina Bourouis
 * \version 0.1
 * \date 09 fevrier 2017
 *
 * Mini Shell de Sabrina Bourouis et Alexis Heil
 */
#include <stdio.h>
#include <stdlib.h>
#include "StringGoodies.h"
#include <assert.h>
#include <string.h>

/**
 * \brief fonction qui permet de supprimer les caracteres blancs en debut et en fin de chaine 
 * \param char * str chaine dont il faut supprimer les espaces 
 * \return char* la chaine modifiee 
 */
char *trimwhitespace(char *str)
{
  char *end;
  // suppression des espaces du debut
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // suppression des espaces de fin
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) 
  {
  		end--;
  }
  // ecriture du caractere de fin de chaine
  *(end+1) = 0;
  return str;
}

/**
 * \brief fonction qui permet de remplacer la chaine rep par with 
 * \param char * orig chaine dans laquelle il faut remplacer rep par with    
 * \param char * rep chaine a remplacer 
 * \param char * with chaine par laquelle on va remplacer rep 
 * \return char* la chaine modifiee 
 */
char *str_replace(char *orig, char *rep, char *with) {
    char *result; 
    char *ins;    
    char *tmp;    
    int len_rep;  
    int len_with; 
    int len_front; 
    int count;    // number of replacements

    // verification des parametres
    if (!orig || !rep)
        return NULL;

    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; 

    if (!with)
        with = "";
    len_with = strlen(with);

    // compte le nombre de deplacement a effectuer
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
    	//substring a partir de rep
        ins = strstr(orig, rep);
        len_front = ins - orig; //recuperation de la longueur de la chaine avant rep
        tmp = strncpy(tmp, orig, len_front) + len_front; // copie dans tmp la chaine avant rep et se place juste avant le premier caractere de rep dans la chaine
        tmp = strcpy(tmp, with) + len_with; // on ajoute la chaine with 
        orig += len_front + len_rep; // on se place apres le rep dans la chaine orig
    }
    strcpy(tmp, orig); // on ajoute le reste des caracteres
    return result;
}

/**
 * \brief fonction qui permet de spliter une chaine par rapport a un caractere 
 * \param char * a_str chaine a spliter    
 * \param char a_delim caractere par rapport auquel il faut spliter 
 * \return char** un tableau de la chaine splitee 
 */
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // compte le nombre d'element on aura au final
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        if(idx != count)
        {
        	return NULL;
        }
        *(result + idx) = (char*)0;
    }
    return result;
}

/**
 * \brief fonction qui permet de verifier si une chaine commence par une autre  
 * \param char * pre chaine par laquelle doit commencer str pour retourner 1    
 * \param char str chaine dont il faut verifier qu'elle commence par pre 
 * \return int 1 si str commence par rep, 0 sinon  
 */
bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? FALSE : strncmp(pre, str, lenpre) == 0;
}
