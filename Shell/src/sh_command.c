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
int execCommand(char **args)
{
	
	//Comparaison entre la première cellule du tableau (qui est obligatoirement une commande) et les commandes
	//Obligé de comparer avec \n derrière
	//Bonne solution ?? Don't know
	int isWorking;
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
		int status_pid; //Status du process fils 
        int dp[2]; // Tableau d'entrée sorti du pipe
        if(pipe(dp) == -1) { // Test d'ouverture de pipe
          perror("Pipe failed");
          //return();
        }
		if(fork() == 0) //Processus fils
		{
			// A faire un pipe
			// REGARDER LA DOC EN PLUS
			dup2(1,2); // Redirection
			dup2(dp[1],1); //Redirige l'entrée standard sur la sortie du pipe
			close(dp[1]); // Fermeture de la sortie du pipe
        	execvp(args[0], args);
            exit(errno);  //Permet de quitter correctement le fils
		}
		else{
			wait(&status_pid);
			close(dp[1]); // Fermeture de la sortie ou l'entrée jsp du pipe
			int size = 1;
			char read_buff[size];
			while(read(dp[0], read_buff, size) > 0)
			{
				write(1, read_buff, size);
			}
			if(WEXITSTATUS(status_pid) ==0)
			{
				return 0;
			}
			return 1;
		}


	}
	//printf("%d", isWorking);
}