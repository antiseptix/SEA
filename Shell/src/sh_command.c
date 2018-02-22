#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sh_command.h>

void sh_pwd(){
	char path[40] = "";
	printf("%s \n" , getcwd(path, sizeof(path)));
}

void sh_exit(){
	exit(0);
}

void sh_cd(char **args){
	char path[40] = "";
	printf("Args[1] %s", args[1]);
	if(args[1] == NULL){
		printf("Minimum un argument requis pour executer CD \n");
	}else{
		//getcwd(path, sizeof(path));
		//strcat(path, "/");
		//strcat(path, args[1]);
		//path = "/tmp";
		printf("CHEMIN %s \n", path);
		chdir(args[1]);
	}
}

void sh_echo(char **args){
	printf("%s", args[1]);
}

void sh_help(){
	printf("Shell réalisé par Arnaud LAPENNA et Julien GROLL \n");
	printf("Commande prise en charge \n");
	// for (int i = 0; i < sizeof(command_str); i++) {
	// 	printf("  %s\n", command_str[i]);
	// }
}

void execCommand(char **args){
	
	//Comparaison entre la première cellule du tableau (qui est obligatoirement une commande) et les commandes
	//Obligé de comparer avec \n derrière
	//Bonne solution ?? Don't know
	
	if (strcmp(args[0], "pwd\n") == 0){
	    sh_pwd();
	}else if(strcmp(args[0], "exit\n") == 0){
		sh_exit();
	}else if (strcmp(args[0], "cd") == 0){
	    sh_cd(args);
	}else if(strcmp(args[0], "echo\n") == 0){
		sh_echo(args);
	}else if(strcmp(args[0], "help\n") == 0){
		sh_help();
	}else{
		system(args[0]);
	}
}