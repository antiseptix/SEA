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
#include <sys/wait.h>
#include <errno.h>
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
	if(args[1] == NULL){
		perror("Erreur");
	}else{
		chdir(args[1]);
	}
}

/**
 * \brief Fonction permettant de lancer la commande ECHO
 * \param char ** args argument de la commande a executer (sous forme de tableau)
 */
void sh_echo(char **args){
	int i = 1;
	if(args[1] != NULL){
		while(args[i] !=NULL){
			printf("%s ", args[i]);
			i++;
		}
		printf("\n");
	}else
		perror("Erreur");
}

/**
 * \brief Fonction permettant de lancer la commande HELP
 */
void sh_help(){
	printf("Shell réalisé par Arnaud LAPENNA et Julien GROLL \n");
	printf("Commande prise en charge :\n");
	printf("PWD | EXIT | CD | ECHO | HELP \n");
	
}

/**
 * \brief Fonction permettant d'excuter une commande reçu
 * \param char **args argument de la commande a executer (sous forme de tableau)
 */
void execCommand(char **args){
	
	//Comparaison entre la première cellule du tableau (qui est obligatoirement une commande) et les commandes
	//Obligé de comparer avec \n derrière
	//Bonne solution ?? Don't know
	
	if (strcmp(args[0], "pwd") == 0){
	    sh_pwd();
	}else if(strcmp(args[0], "exit") == 0){
		sh_exit();
	}else if (strcmp(args[0], "cd") == 0){
	    sh_cd(args);
	}else if(strcmp(args[0], "echo") == 0){
		sh_echo(args);
	}else if(strcmp(args[0], "help") == 0){
		sh_help();
	}else{
		int status_pid;
		if(fork() == 0){
        	execvp(args[0], args);
        	perror("Erreur ");
            exit(1);
		}
		wait(&status_pid);
	}
}