/**
 * \file builtin.c
 * \brief Fichier permettant de lancer un MiniShell
 * \author Alexis Heil, Sabrina Bourouis
 * \version 0.1
 * \date 09 fevrier 2017
 *
 * Mini Shell de Sabrina Bourouis et Alexis Heil
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <StringGoodies.h>
#include "builtin.h"

/**
 * \brief Tableau de char* qui contient les differentes commandes qui ont ete implementees directement
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "pwd", 
  "echo",
  "history"
};

/**
 * \brief Tableau de pointeur de fonctions qui retourne un int et prenne un char ** en parametre. Chacune de ces fonctions correspond a une des commandes implementees directement
 */
int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &lsh_pwd,
  &lsh_echo,
  &lsh_history
};

/**
 * \brief fonction qui retourne le nombre d'element dans le tableau builtin_str
 * \return int taille du tableau builtin_str
 */
int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/**
 * \brief fonction qui correspond a l'implementation de la fonction cd
 * \param char ** args argument de la commande a executer le premier element du tableau est cd 
 * \return int code retour de la fonction 1 si la fonction se termine bien et 0 sinon 
 */
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "Un argument est requis pour \"cd\"\n");
    return 0;
  } else {
    if (chdir(args[1]) != 0) {
      perror("Erreur");
      return 0;
    }
  }
  return 1;
}

/**
 * \brief fonction qui correspond a l'implementation de la fonction help
 * \param char ** args argument de la commande a executer le premier element du tableau est help
 * \return int code retour de la fonction : 1 (elle effectue juste un affichage) 
 */
int lsh_help(char **args)
{
  int i;
  printf("Alexis and Sabrina's Shell\n");
  printf("Entrer le nom des commandes et taper sur entrer \n");
  printf("Les commandes suivantes ont été implémentées directement :\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Utiliser le man pour les autres commandes.\n");
  return 1;
}

/**
 * \brief fonction qui correspond a l'implementation de la fonction exit
 * \param char ** args argument de la commande a executer le premier element du tableau est exit [Pas utilise juste pour correspondre a la definition du tableau de fonction] 
 * \return int code retour de la fonction : 0  
 */
int lsh_exit(char **args)
{
  exit(0);
  return 0;
}

/**
 * \brief fonction qui correspond a l'implementation de la fonction pwd
 * \param char ** args argument de la commande a executer le premier element du tableau est pwd [Pas utilise juste pour correspondre a la definition du tableau de fonction] 
 * \return int code retour de la fonction : 1  
 */
int lsh_pwd(char ** args)
{
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s\n", cwd);
  return 1;
}

/**
 * \brief fonction qui correspond a l'implementation de la fonction echo
 * \param char ** args argument de la commande a executer le premier element du tableau est echo [Pas utilise juste pour correspondre a la definition du tableau de fonction] 
 * \return int code retour de la fonction : 1  
 */
int lsh_echo(char ** args)
{
  int i = 1;
  while(args[i] != NULL)
  {
    printf("%s ", args[i] );
    i++;
  }
  printf("\n");
  return 1;
}

/**
 * \brief fonction qui correspond a l'implementation de la fonction history
 * \param char ** args argument de la commande a executer le premier element du tableau est history [Pas utilise juste pour correspondre a la definition du tableau de fonction] 
 * \return int code retour de la fonction 1 si la fonction se termine bien et 0 sinon 
 */
int lsh_history(char ** args)
{
  char * nomHistorique = malloc(sizeof(char)*(sizeof(HISTORY_NAME)+sizeof(getenv("HOME"))+2));

  /*
   * creation du chemin du fichier d'historique
   */
  strcpy(nomHistorique, getenv("HOME"));
  chdir(nomHistorique);
  strcat( nomHistorique, "/");
  strcat( nomHistorique, HISTORY_NAME);

  //ouverture du fichier historique et creation si l'utilisateur l'a supprime lors de l'execution du shell
  int fdHistorique = open(nomHistorique, O_RDONLY | O_CREAT);

  if (fdHistorique < 0)
  {
    perror("Erreur");
    return 0;
  }

  int number_of_lines = 1; // permet d'afficher le numero de la ligne comme le fait la commande history
  printf("%d   ", number_of_lines);
  char ch;
  lseek(fdHistorique,0,SEEK_SET);
  //lecture du fichier historique caractere par caractere et affichage sur la sortie standard
  while (read(fdHistorique, &ch, 1) == 1)
  {
    printf("%c",ch);
    if(ch == '\n' && number_of_lines < NB_LINE_HISTORY)
    {
        number_of_lines++;
        printf("%d   ", number_of_lines);
    }
  } 

  close(fdHistorique);
  return 1;
}

/**
 * \brief fonction qui permet de selectionner la bonne commande builtin a executer 
 * \param char * args argument de la commande a executer le premier element du tableau est history [Pas utilise juste pour correspondre a la definition du tableau de fonction] 
 * \return int code retour de la fonction 1 si la fonction se termine bien et 0 sinon 
 */
int execBuiltin(char * arg)
{
  int i;
  char ** args = str_split(trimwhitespace(arg), ' ');
  if (args == NULL) 
  {
    return 0;
  }
  //parcours du tableau contenant le nom des fonctions builtin
  for (i = 0; i < lsh_num_builtins(); i++) 
  {
  	//compare avec le premier parametre
    if (strcmp(trimwhitespace(args[0]), builtin_str[i]) == 0) 
    {
      return (*builtin_func[i])(args);
    }
  }
  return 0;
}
