/**
 * \file sh_command.c
 * \brief Fichier contenant les fonctions pour l'execution des commande built-in de notre Shell
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
 * \brief Fonction permettant de lancer la commande PWD
 */
void sh_pwd(){
	char path[40] = "";
	printf("%s \n" , getcwd(path, sizeof(path)));
}

/**
 * \brief Fonction permettant de lancer la commande EXIT
 */
void sh_exit(){
	exit(0);
}

/**
 * \brief Fonction permettant de lancer la commande CD
 * \param char ** args argument de la commande a executer (sous forme de tableau)
 */
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

/**
 * \brief Fonction permettant de lancer la commande ECHO
 * \param char ** args argument de la commande a executer (sous forme de tableau)
 */
void sh_echo(char **args){
	printf("%s", args[1]);
}

/**
 * \brief Fonction permettant de lancer la commande HELP
 */
void sh_help(){
	printf("Shell réalisé par Arnaud LAPENNA et Julien GROLL \n");
	printf("Commande prise en charge \n");
	// for (int i = 0; i < sizeof(command_str); i++) {
	// 	printf("  %s\n", command_str[i]);
	// }
}

/**
 * \brief Fonction permettant d'excuter une commande reçu
 * \param char **args argument de la commande a executer (sous forme de tableau)
 */
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