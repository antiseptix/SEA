/**
 * \file shell.c
 * \brief Fichier contenant le main de notre Shell
 * \author Arnaud Lapenna, Julien Groll
 * \version 0.1
 * \date 23 fevrier 2018
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sh_command.h>
#define yellow "\033[33;1m"
#define white "\033[37;0m"

/**
 * \brief Fonction permettant la recherche d'un caractère dans une chaine
 * \param char* string Chaine à chercher char* speChar Caractère de recherche
 * \return Return 1 si le carctère est trouvé sinon 0 
 */
int  searchChar(char *string, char *speChar)
{
	char *temp;
	temp = strstr(string, speChar);
	if(temp != NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * \brief Fonction permettant d'écrire la commande dans un fichier
 */
void writeHistory(char *input)
{
	FILE * file;
	
	//Ouverture du fichier en mode création (si il n'existe pas) , écriture seule et à la fin du fichier
	file = fopen("history.txt", "a+");
	
	//Ecriture dans le fichier avec la variable input
	fprintf(file, "%s", input);
	
	//Fermeture du fichier
	fclose(file);
}
/**
 * \brief Fonction permettant d'afficher le chemin courrant dans notre shell
 * \param string to write in the actual path at the call
 * \param size of the variable (sizeof(string)) 
 */
void displayPrompt(char *path,size_t size){
	//Récupère le chemin courrant
	getcwd(path,size);
	//Print le chemin courrant en couleur
	printf("%s %s : %s", yellow, path, white);
}

/**
 * \brief Fonction permettant de lancer le shell
 */
int main(int argc, char *argv[]){
	//déclaration de la var path[] qui va etre reutilisé à chaque boucle (évite la réinstanciation multiple)
	char path[128] = "";
	size_t sizeOfPath = sizeof(path);
	//Verification que le premier argument existe
	if(argv[1] != NULL)
	{
		// Recherche du -b pour le lancer en mode commande simple du tpye : "./shell -b cd Document"
		if(strcmp(argv[1], "-b") == 0)
		{
			// Il faudra surement changer le system et remplacer par le lancement de commande fait maison
		}
	}
	else
	{		// Si le -b n'est pas trouvé on lance le shell
		char input[512] = "";

		//Boucle du shell
		while (1) {
			displayPrompt(path,sizeOfPath);
			
			//Récupération des instructions de l'utilisateur
			fgets(input, sizeof(input), stdin);
			
			writeHistory(input);
			char *result = NULL;
	        // Initialisation du tableau 
	        //http://compsci.ca/v3/viewtopic.php?t=26591 <== à voir pour comprendre le**
	        int i = 0;
			char **args = malloc(10 * sizeof(char *));
			for (i=0; i<10; i++)
			{
			    args[i] = malloc(40 * sizeof(char));
			}
			//Permet de changer \n en \0 
			input[strlen(input)-1] = '\0';
			//Découpage de la chaine à chaque espace
		    result = strtok(input, " ");
		    i = 0;
			while (result != NULL)
			{
				//Copie dans la chaine dans un tableau d'argument
				strcpy(args[i], result);
				i++;
				result = strtok( NULL, " ");
			}
			// Ajout au niveau du dernier tableau NULL pour préciser la fin du tableau pour l'execvp
			args[i] = NULL;
			
			//On lance l'execution de la commande que si l'user a saisit quelque-chose (no null or only whitespace)
			if (args[0]!= NULL){
				if(searchChar(args[0], "&") == 1)
				{
					// Exec du thread
					// Pour l'exec en arrière plan : ls -l & 
					// Le & est toujours à la fin donc à changer le args[0]
				}
				else
				{
					// Exec fonction
					execCommand(args);
				}
			}			
		}
	}
	return 1;
}