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
#include "sh_command.h"

/**
 * \brief Fonction permettant de lancer la commande PWD
 */
int sh_pwd(){
	char path[128] = "";
	printf("%s \n" , getcwd(path, sizeof(path)));
	return 0;
}

/**
 * \brief Fonction permettant de lancer la commande EXIT
 */
int sh_exit()
{
	exit(0);
	return 0;
}

/**
 * \brief Fonction permettant de lancer la commande CD
 * \param char ** args argument de la commande a executer (sous forme de tableau)
 */
int sh_cd(char **args)
{
	if(args[1] == NULL)
	{
		perror("Erreur");
		return 1;
	}
	else
	{
		chdir(args[1]);
		return 0;
	}
}

/**
 * \brief Fonction permettant de lancer la commande ECHO
 * \param char ** args argument de la commande a executer (sous forme de tableau)
 */
int sh_echo(char **args)
{
	int i = 1;
	if(args[1] != NULL)
	{
		while(args[i] !=NULL)
		{
			printf("%s ", args[i]);
			i++;
		}
		printf("\n");
		return 1;
	}else
		perror("Erreur");
		return 0;
}

/**
 * \brief Fonction permettant de lancer la commande HELP
 */
int sh_help()
{
	printf("Shell réalisé par Arnaud LAPENNA et Julien GROLL \n");
	printf("Commande prise en charge :\n");
	printf("PWD | EXIT | CD | ECHO | HELP \n");
	return 1;
}

/**
 * \brief Fonction permettant d'excuter une commande reçu
 * \param char **args argument de la commande a executer (sous forme de tableau)
 */
void execCommand(char **args)
{
	
	//Comparaison entre la première cellule du tableau (qui est obligatoirement une commande) et les commandes
	//Obligé de comparer avec \n derrière
	//Bonne solution ?? Don't know
	int isWorking=0; //######################################################################################################"A ENLEVER"
	//int isWorking;
	
	if (strcmp(args[0], "pwd") == 0)
	{
	     isWorking = sh_pwd();
	}
	else if(strcmp(args[0], "exit") == 0)
	{
		isWorking = sh_exit();
	}
	else if (strcmp(args[0], "cd") == 0)
	{
	    isWorking = sh_cd(args);
	}
	else if(strcmp(args[0], "echo") == 0)
	{
		isWorking = sh_echo(args);
	}
	else if(strcmp(args[0], "help") == 0)
	{
		isWorking = sh_help();
	}
	else
	{
		int status_pid;
		
		if(fork() == 0)
		{
			//Processus fils
			// A faire un pipe
			
        	isWorking = execvp(args[0], args);
        	perror("Erreur ");
        	
            exit(1);
		}
		wait(&status_pid);
		
		isWorking++; // #######################################################################################################" A ENLEVER"

	}
}