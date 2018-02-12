#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
	int c = 1;
	char input[64] = "";

	while (c==1) {
		displayPrompt();
		//Récupération des instructions de l'utilisateur
		fgets(input, sizeof(input), stdin);

		char *result = NULL;
        // Initialisation du tableau 
		char **tab = malloc(10 * sizeof(char *));
		for (int i=0; i<10; i++){
		    tab[i] = malloc(40 * sizeof(char));
		}

	    result = strtok(input, " ");
	    int i = 0;
		while (result != NULL){
		//Découpage de la chaine pour la mettre un tableau
			strcpy(tab[i], result);
			i++;
			//printf("%s\n", result);
			result = strtok( NULL, " ");
		}

		//printf("%s \n", tab[0]);
		
		//Comparaison entre la première cellule du tableau (qui est obligatoirement une commande) et les commandes
		if (strcmp(tab[0], "pwd\n") == 0){
		    pwd();
		}else if (strcmp(tab[0], "cd\n") == 0){
		    printf("CD \n");
		}else if(strcmp(tab[0], "ls\n") == 0){
		    system("ls");
		}else if(strcmp(tab[0], "exit\n") == 0){
			sh_exit();
		}else{
			printf("--Commande inconnue veuillez retaper une commande-- \n");
		}
	}
}

void displayPrompt(){
	char path[40] = "";
	//Récupère le chemin courrant
	getcwd(path, sizeof(path));
	//Print le chemin courrant en couleur
	printf("\033[34m %s : \033[37m", path);
}

void pwd(){
	char path[40] = "";
	printf("%s \n" , getcwd(path, sizeof(path)));
}

void sh_exit(){
	exit(0);
}