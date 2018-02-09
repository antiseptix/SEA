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
		int i = 0;
		char *tab[40] = malloc(40*sizeof(char));

	    result = strtok(input, " ");

		while (result != NULL){
		//Découpage de la chaine pour la mettre un tableau
			strcpy(tab[i], result);
			i++;
			printf("%s\n", result);
			result = strtok( NULL, " ");
		}
		printf("Dans le tableau : %s\n", tab[0]);
	}
}

void displayPrompt(){
	char path[40] = "";
	//Récupère le chemin courrant
	getcwd(path, sizeof(path));
	//Print le chemin courrant en couleur
	printf("\033[34m %s : \033[37m", path);
}
