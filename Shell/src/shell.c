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

/**
 * \brief Fonction permettant de lancer le shell
 */
int main(int argc, char *argv[]){
	//Verification que le premier argument existe
	if(argv[1] != NULL){
		// Recherche du -b pour le lancer en mode commande simple du tpye : "./shell -b cd Document"
		if(strcmp(argv[1], "-b") == 0){
			// Il faudra surement changer le system et remplacer par le lancement de commande fait maison
			execv
		}
	}else{		// Si le -b n'est pas trouvé on lance le shell
		int c = 1;
		char input[512] = "";
	
		//Ouverture du fichier en mode création (si il n'existe pas) , écriture seule et à la fin du fichier
		int file = open("hystory.txt", O_CREAT | O_RDWR | O_APPEND);

		//Boucle du shell
		while (c==1) {
			displayPrompt();
			
	
			//Récupération des instructions de l'utilisateur
			fgets(input, sizeof(input), stdin);
			//Ecriture dans le fichier avec la variable input
			write(file, input, strlen(input));
	
			char *result = NULL;
	        // Initialisation du tableau 
	        //http://compsci.ca/v3/viewtopic.php?t=26591 <== à voir pour comprendre le**
	        int i = 0;
			char **args = malloc(10 * sizeof(char *));
			for (i=0; i<10; i++){
			    args[i] = malloc(40 * sizeof(char));
			}
	
		    result = strtok(input, " ");
		    i = 0;
			while (result != NULL){
			//Découpage de la chaine pour la mettre un tableau
				strcpy(args[i], result);
				i++;
				//printf("%s\n", result);
				result = strtok( NULL, " ");
			}
			execCommand(args);
		}
		//Fermeture du fichier
		close(file);
	}
return 1;
}	

/**
 * \brief Fonction permettant d'afficher le chemin courrant dans notre shell
 */
void displayPrompt(){
	char path[40] = "";
	//Récupère le chemin courrant
	getcwd(path, sizeof(path));
	//Print le chemin courrant en couleur
	printf("\033[34m %s : \033[37m", path);
}