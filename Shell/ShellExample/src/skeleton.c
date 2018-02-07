/**
 * \file skeleton.c
 * \brief Fichier permettant de lancer un MiniShell
 * \author Alexis Heil, Sabrina Bourouis
 * \version 0.1
 * \date 09 fevrier 2017
 *
 * Mini Shell de Sabrina Bourouis et Alexis Heil
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <liste_chaine.h>
#include <arbr.h>
#include <StringGoodies.h>
#include <builtin.h>
#include <sys/stat.h> 
#include <assert.h>

#define COPIE_FILENAME "copie.txt"

#define TRUE 1
#define FALSE 0

typedef int bool; 

/**
 * Procedure permettant de verifier si la variable doit etre liberee
 * (check: ptr != NULL)
 *
 * \param void* to_free pointeur vers une adresse memoire allouee grace a malloc, realloc,...
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}

/**
 * Fonction qui verifie l'existence d'un fichier
 *
 * \param char* ptrFile nom du fichier dont il faut tester l'existence
 * \return int 1 = le fichier existe, 0 = le fichier n'existe pas 
 */
int fileExists(char *ptrFile)
{
  struct stat s;
       
  if (stat(ptrFile, &s) == 0)
    return 1 ;
  else
    return 0 ;
} 

/**
 * Fonction qui permet d'ajouter un noeud a l'arbre passe en parametre 
 * Noeud : Racine = c, Feuille gauche = l, Feuille droite = a
 * \param char c caractere a ajoute a la racine 
 * \param liste l liste chainee a ajoute en tant que feuille gauche 
 * \param arbre a arbre auquel on va ajouter le noeud 
 * \return arbre une reference vers la racine de l'arbre 
 */
arbre addNode(char c, liste l, arbre a)
{
  /*
   *  Exemple d'arbre :
   *
   *
   *             &&
   *            /  \
   *           /    \
   *         ls      ||
   *                /  \
   *               /    \
   *    echo ça marche   ;
   *                    / \
   *                   /   \
   *   echo ça marche pas   NULL
   */ 
  arbre res = a;
  //verifie si l'arbre est vide, si c'est le cas on cree un nouvel arbre 
  if (estVideA(a))
  {
    //ajout de la valeur courante a la racine
    liste curVal = nouvListe();
    adjt(c, curVal );
    //creation de l'arbre Racine : c, Feuille gauche = l, Feuille droite = NULL
    res = enracA(curVal, creerFeuilleA(l), nouvArbre());
  }
  //si la feuille gauche est vide on ajoute le noeud a la feuille gauche 
  else if(estVideA(fgA(a)))
  {
    addNode(c, l, fgA(a));
  }
  //permet de gerer les caracteres doubles cad : &&, ||, <<, >> on modifie donc la racine courante pour y ajouter c et donc doubler le caractere 
  else if(estVideA(fdA(a)) && a->val != NULL && a->val->premier != NULL && a->val->premier->val == c && taille(a->val) < 2 && 
    ( (l == NULL || l->premier == NULL) && (((a->val->premier->val == '&') ) || (a->val->premier->val == '|')  || (a->val->premier->val == '>') || (a->val->premier->val == '<'))))
  {
    adjq(c, a->val);
  }
  //si on se trouve dans aucun des precedents cas on rappelle la fonction pour ajouter le noeud dans le sous-arbre de droite
  else
  {
    a->fd = addNode(c, l, fdA(a));
  }
  return res;
}

/**
 * Fonction qui permet de recuperer les variables d'envirronnement dans envp
 * 
 * \param char * name nom de la variable d'environnement dont on souhaite recuperer la valeur 
 * \param char ** envp contient une entree pour chaque varaible d'environnement Ex : Path=valueOfPAth
 * \return char* la valeur de la variable d'environnement
 */
char * getEnvVar(char * name, char ** envp)
{
  short i = 0;
  bool found = FALSE;
  char * res;
  //parcourt envp
  while(!found && envp[i] != NULL)
  {
    //verifie si l'entree courante commence par le nom de la variable d'environnement
    if(startsWith(name, envp[i]))
    {
      res = str_replace(envp[i],name, "");
      found = TRUE;
    }
    else
    {
      i++;
    }
  }
  return res;
}

/**
 * Fonction qui permet de recuperer le fichier contenant la commande a execute necessaire pour execve
 *
 * \param char * cmd nom de la commande qu'il faut trouver 
 * \param char ** envp contient une entree pour chaque varaible d'environnement necessaire pour retrouver la commande Ex : Path=valueOfPAth
 * \return char* chemin vers la commande a executer
 */
char * getCommande(char * cmd, char ** envp)
{
  //recuperation du premier chemin dans la variable PATH
  char *token = strtok(getEnvVar("PATH=", envp), ":");
  char * fileName = (char *)malloc(sizeof(char)*(strlen(token)+strlen(cmd))); // variable qui va contenir le chemin du fichier
  bool found = FALSE; // variable de bascule

  while( !found && token != NULL ) 
  {
    //creation du chemin du fichier dans fileName
    strcpy(fileName, token);
    strcat(fileName, "/"); 
    strcat(fileName, cmd);
    
    //verification de l'existence du fichier
    if(!fileExists(fileName))
    {
      //recuperation du prochain chemin de la variable PATH
      free_if_needed(fileName); 
      token = strtok(NULL, ":");     
      fileName = (char *)malloc(sizeof(char)*(strlen(token)+strlen(cmd)));  
    }
    else
    {
      found = TRUE;
    }
  }
  return fileName;
}

/**
 *  Fonction qui permet de lire une ligne saisi au clavier
 * \return char* la ligne saisie au clavier
 */
char *read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

/**
 *  Fonction qui va demander a l'utilisateur de saisir des donnees a inserer dans le fichier fd tant que la valeur tag n'a pas ete saisie
 *
 * \param char * tag valeur que l'utilisateur doit saisir pour terminer la lecture 
 * \param int fd descripteur du fichier dans lequel les donnees vont etre inseree 
 */
void askUntilTag(char * tag, int fd)
{
  char * line = read_line();
  while(strcmp(trimwhitespace(line), trimwhitespace(tag)) != 0)
  {
    dprintf(fd,"%s\n", line);
    line = read_line();
  }
}

/**
 *  Fonction qui va recuperer le contenu d'un fichier et le passer en argument de la commande a executer
 *
 * \param char * cmd commande que l'utilisateur a tape 
 * \param char ** envp tableau des variables d'environnement
 * \param char * fileName nom du fichier dans lequel il faut recuperer les donnees 
 * \param int append booleen qui permet de savoir s'il faut demander a l'utilisateur de saisir les donnees au clavier jusqu'à un tag 
 * \return int code retour de la commande a executer
 */
int execRedirectFromFile(char * cmd, char ** envp, char * fileName, int append)
{
  char ** tmp = str_split(cmd, ' '); 
  int status;
  pid_t pid = fork();

  if(pid == 0)
  {
    int fd;
    if(append)
    {
      //on ouvre le fichier fileName 
      fd = open(trimwhitespace(fileName), O_RDONLY );
      if (fd < 0)
      {
        perror("Erreur ");
        exit(-1);
      }

      //on specifie le fichier comme etant l'entree standard
      dup2(fd, 0);
      close(fd);
    }
    else
    {
      //creation d'un fichier temporaire
      char template[] = "/tmp/MySHELL_XXXXXX";
      fd = mkstemp(template);

      if (fd < 0)
      {
        perror("Erreur ");
        exit(-1);
      }

      //demande a l'utilisateur de rentrer les donnees
      askUntilTag(fileName, fd);
      //on se remet au debut du fichier
      lseek(fd, 0, SEEK_SET);
      //definit le fichier comme l'entree standard
      dup2(fd, 0);
      close(fd);

    }

    execve(getCommande(tmp[0], envp), tmp, envp );
    perror("Erreur ");
    exit(-1);

  }
  else if (waitpid(pid, &status, 0) != -1  ) // attend la fin du processus
  {
    return (WIFEXITED( status ) && WEXITSTATUS( status ) == 0);
  }
  return 0;
}

/**
 *  Fonction qui va rediriger l'affichage dands un fichier 
 *
 * \param char * cmd commande que l'utilisateur a tape 
 * \param char ** envp tableau des variables d'environnement
 * \param char * fileName nom du fichier dans lequel on va ecrire les donnees 
 * \param int append booleen qui permet de savoir si on ajoute ou on remplace le contenu
 * \return int code retour de la commande a executer
 */
pid_t execRedirectToFile(char * cmd, char ** envp, char * fileName, int append)
{
  char ** tmp = str_split(cmd, ' '); 
  int status;
  pid_t pid = fork();

  if(pid == 0)
  {
    //ouvrir le fichier en mode ecriture ou en mode ajout
    int fd = open(trimwhitespace(fileName), append ? O_WRONLY|O_APPEND : O_WRONLY );
    if (fd < 0)
    {
      perror("Erreur ");
      exit(-1);
    }
    //redirection de la sortie standard et de la sortie d'erreur
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);

    //verifie s'il ne s'agit pas d'une fonction builtin
    if ( !execBuiltin(cmd))
    {
      execve(getCommande(tmp[0], envp), tmp, envp );
      perror("Erreur ");
      exit(-1);
    }
    else
    {
      exit(1);
    }

  }
  else if (waitpid(pid, &status, 0) != -1  ) // attend la fin du processus
  {
    return (WIFEXITED( status ) && WEXITSTATUS( status ) == 0);
  }
  return 0;
}

/**
 *  Fonction qui va executer la commande l 
 *
 * \param liste l commande que l'utilisateur a saisi 
 * \param char ** envp tableau des variables d'environnement
 * \return int code retour de la commande a executer
 */
int exec(T l, char ** envp )
{
  char * cmd = trimwhitespace(getString(l));
  //copie car la fonction builtin modifie la chaine
  char * cmdCopie = malloc(sizeof(char)*strlen(cmd));
  strcpy(cmdCopie, cmd);

  if ( execBuiltin(cmdCopie))
  {
    return 1;
  }

  int status;
  pid_t pid = fork();
  if( pid == 0)
  {
    char ** tmp = str_split(cmd, ' ');
    execve(getCommande(tmp[0], envp), tmp, envp );
    perror("Erreur ");
    exit(-1);
  }
  if (waitpid(pid, &status, 0) != -1  ) // attend la fin du processus
  {
    return (WIFEXITED( status ) && WEXITSTATUS( status ) == 0);
  }
  return 0;
}

/**
 *  Fonction qui va executer toutes les commandes stockees dans l'arbre a
 *
 * \param arbre a arbre a executer
 * \param char ** envp tableau des variables d'environnement
 * \param int worked pour connaitre le code retour de la derniere commande executee
 * \param int executeNext pour savoir si la commande de la feuille gauche doit etre executee
 * \return int code retour de la commande a executer
 */
void execArbre2(arbre a, char** envp, int worked, int executeNext)
{
  //si l'arbre est vide on ne fait rien
  if(estVideA(a) || fgA(a) == NULL)
  {
    return;
  }

  int status;
  pid_t pid;
  char * sep = getString(a->val);
  //executeNext est false la commande de la feuille gauche ne doit pas etre executee et worked est false donc la suivante ne doit
  // etre execute que si le separateur est || ou ;
  if(!executeNext && worked == 0)
  {
    //on execute l'arbre de la feuille droite en changeant les parametres : worked identique on a pas essaye d'executer de commande; 
    //executeNext peut changer en fonction de la valeur du séparateur
    execArbre2(fdA(a), envp, worked, (strcmp(sep, "||") == 0 || strcmp(sep, ";") == 0));
  }
  //executeNext est false la commande de la feuille gauche ne doit pas etre executee et worked est vrai ou ND donc la suivante ne doit
  // etre execute que si le separateur est different de ||
  else if(!executeNext && worked != 0)
  {
    //on execute l'arbre de la feuille droite en changeant les parametres : worked identique on a pas essaye d'executer de commande; 
    //executeNext peut changer en fonction de la valeur du séparateur
    execArbre2(fdA(a), envp, worked, (strcmp(sep, "||") != 0 ));
  }
  //execute next est vrai donc on execute la feuille gauche
  else if ( executeNext && (strcmp(sep, "&&") == 0 || strcmp(sep, "||") == 0 || strcmp(sep, ";") == 0 )) 
  {
    if( (status = exec(fgA(a)->val, envp))) // execute la commnde
    { 
      execArbre2(fdA(a), envp,  1, strcmp(sep, "||") != 0  );
    }
    else 
    {
      // execute la commande suivante avec en parametre worked 0 et executenext en fonction du separateur courant
      execArbre2(fdA(a), envp, 0, strcmp(sep, "||") == 0 || strcmp(sep, ";") == 0);
    }
  }
  //execute next est vrai donc on execute la feuille gauche et si le separateur est unpipe 
  else if(executeNext && strcmp(sep, "|") == 0)
  {
    int pipefd[2]; // tab pour le pipe
    pid_t pid1;
    pipe(pipefd); // creation du pipe
    
    //execution de la commande d'ecriture dans le pipe
    if ( (pid1 = fork()) == 0 ) {
      close(pipefd[0]); // on ferme la partie ecriture du pipe

      if (dup2(pipefd[1], 1) == -1 )  // on remplace la sortie standard par notre pipe partie output
      {
        perror("Erreur ");
        exit(-1);
      } 
      close(pipefd[1]);
      exec(fgA(a)->val, envp); // on execute notre commande
      
      exit(1); // on sort du process
    }
    //execution de la commande de lecture dans le pipe
    else
    {
      if ((pid = fork()) == 0 ){
        close(pipefd[1]); // on ferme la partie output du pipe
        if (dup2(pipefd[0], 0) == -1 )  // on remplace l'entrée standard par notre pipe partie input
        {
          perror("Erreur ");
          exit(-1);
        }

        execArbre2(fdA(a), envp, 1, TRUE); // on execute la commande suivante
        close(pipefd[0]);
        exit(1); // on sort du process
      }

      close(pipefd[1]); // on ferme notre pipe
      close(pipefd[0]);

      waitpid(pid1, &status, 0); // on attend l'execution des process avant de sortir
      waitpid(pid, &status, 0);
    }
  }
  //execute next est vrai donc on execute la feuille gauche et si le separateur est une redirection
  else if( executeNext && (strcmp(sep, ">>") == 0 || strcmp(sep, ">") == 0))
  {
    char * cmd = trimwhitespace(getString(fgA(a)->val));
    //verifie si un fichier est bien specifie
    if (fgA(fdA(a)) != NULL)
    {
      //execution de la commande qui gere la redirection
      status = execRedirectToFile(cmd, envp, getString(fgA(fdA(a))->val), strcmp(sep, ">>") == 0);
      if(fdA(a) != NULL)
      {
        //on continue l'execution de l'arbre
        execArbre2(fdA(fdA(a)), envp, status, (!status && strcmp(getString(fdA(a)->val), "||") == 0) || (status && strcmp(getString(fdA(a)->val), "||") != 0));
      }

    }
  }
  //execute next est vrai donc on execute la feuille gauche et si le separateur est
  else if(executeNext && (strcmp(sep, "<<") == 0 || strcmp(sep, "<") == 0))
  {
    char * cmd = trimwhitespace(getString(fgA(a)->val));
    //char ** tmp = str_split(cmd, ' '); 
    if (fgA(fdA(a)) != NULL)
    {
      //gestion du pipe apres la redirection
      if( strcmp(getString(fdA(a)->val), "|") == 0)
      {
        int pipefd[2]; // tab pour le pipe
        pid_t pid1;
        pipe(pipefd); // creation du pipe
      
        if ( (pid1 = fork()) == 0 ) {
          close(pipefd[0]); // on ferme la partie ecriture du pipe

          if (dup2(pipefd[1], 1) == -1 )  // on remplace l'entrée standard par notre pipe partie input
          {
            perror("Erreur ");
            exit(-1);
          } // on remplace la sortie standard par notre pipe partie output
          close(pipefd[1]);
          status = execRedirectFromFile(cmd, envp, getString(fgA(fdA(a))->val), strcmp(sep, "<") == 0); // on execute notre commande

          exit(1); // on sort du process
        }
        else
        {

          if ((pid = fork()) == 0 ){
            close(pipefd[1]); // on ferme la partie output du pipe
            if (dup2(pipefd[0], 0) == -1 )  // on remplace l'entrée standard par notre pipe partie input
            {
              perror("Erreur ");
              exit(-1);
            }

            execArbre2(fdA(fdA(a)), envp, 1,  strcmp(getString(fdA(a)->val), "||") != 0) ;
            close(pipefd[0]);
            exit(1); // on sort du process
          }

          close(pipefd[1]); // on ferme notre pipe
          close(pipefd[0]);

          waitpid(pid1, &status, 0); // on attend l'execution des process avant de sortir
          waitpid(pid, &status, 0);
        }
      }
      else
      {
        status = execRedirectFromFile(cmd, envp, getString(fgA(fdA(a))->val), strcmp(sep, "<") == 0);
        execArbre2(fdA(fdA(a)), envp, status, (!status && strcmp(getString(fdA(a)->val), "||") == 0) || (status && strcmp(getString(fdA(a)->val), "||") != 0));
      }
    }
  }
  else
  {
    execArbre2(fdA(a), envp, 1, FALSE );
  }
}

/**
 *  Fonction qui va supprimer la premiere ligne d'un fichier
 *
 * \param int fdHistorique filedescripteur du fichier dont on doit supprimer la premiere ligne
 * \return int le filedescripteur du fichier dont on a supprime la premiere ligne
 */
int removeFirstLine(int fdHistorique)
{
  //recuperation du home
  char * home = malloc(sizeof(char)*(sizeof(COPIE_FILENAME)+sizeof(getenv("HOME"))+2));
  char cwd[1024];
  //cration du fichier de copie
  strcpy(home, getenv("HOME"));
  strcat(home, "/");
  strcat(home, COPIE_FILENAME);

  //on se place au debut du fichier dont on souhaite supprimer la premiere ligne
  lseek(fdHistorique,0,SEEK_SET);

  //on cree le fichier dans lequel on va copier l'historique
  int ret = open(home, O_WRONLY | O_CREAT, 0666);
  char ch;

  //on passe lapremiere ligne
  while (read(fdHistorique, &ch, 1) == 1 && ch != EOF && ch != '\n')
  {  }

  //on copie chaque caractere
  while (read(fdHistorique, &ch, 1) == 1 && ch != EOF)
  {
    dprintf(ret, "%c", ch);
  }
  //on ferme les fichiers
  close(fdHistorique);
  close(ret);
  //stocke le repertoire courant 
  getcwd(cwd, sizeof(cwd));
  //on va dans le home
  chdir(getenv("HOME"));
  //on supprime le fichier original
  remove(HISTORY_NAME);
  //on renome la copie comme l'original
  rename(COPIE_FILENAME, HISTORY_NAME);
  //on se replace dans le bon repertoire
  chdir(cwd);
  //on ouvre le fichier 
  home = realloc(home, sizeof(char)*(sizeof(HISTORY_NAME)+sizeof(getenv("HOME"))+2));
  strcpy(home, getenv("HOME"));
  strcat(home, "/");
  strcat(home, HISTORY_NAME);
  ret = open(home, O_RDWR | O_CREAT, 0666);
  free_if_needed(home);
  return ret;
}

/**
 *  Fonction qui va supprimer la premiere ligne du fichier Historique s'il contient plus de NB_LINE_HISTORY
 *
 * \param int fdHistorique filedescripteur du fichier dont on verifie le nombre de ligne
 * \return int le filedescripteur du fichier dont on verifie le nombre de ligne
 */
int checkhistory(int fdHistorique)
{
  int number_of_lines = 0;
  char ch;
  lseek(fdHistorique,0,SEEK_SET);
  while (read(fdHistorique, &ch, 1) == 1)
  {
    if(ch == '\n')
        number_of_lines++;
  } 

  if(ch != '\n' && number_of_lines != 0) 
      number_of_lines++;

  if (number_of_lines > NB_LINE_HISTORY)
  {
    fdHistorique = removeFirstLine(fdHistorique);
  }
  return fdHistorique;
}

/**
 *  Fonction qui verifie s'il y a un caractere & seul dans l'arbre, ce qui signifie que le programme doit etre executer en background
 *
 * \param arbre curArbre arbre dans lequel on verifie s'il y a &
 * \return int 0 si & pas present 1 si present
 */
int isBackground(arbre curArbre)
{
  int res = 0;
  arbre arb = curArbre;
  char * s = trimwhitespace(getString(arb->val));
  while(s != NULL && strcmp(s, "\n") != 0)
  {
    if(strcmp(s, "&") == 0)
    {
      res = 1;
    }
    arb = fdA(arb);
    s = arb == NULL ? NULL : trimwhitespace(getString(arb->val));
  }
  return res;
}

/**
 * fonction qui permet de creer un arbre a partir d'une chaine de caractere
 * \param char* argv chaine de caracteres contenant la commande
 * \param int fdHistorique descripteur du fichier historique
 */
arbre createTreeFromArg(char * argv, int fdHistorique)
{
  int t= 0, bascule = 1;
  arbre arb = nouvArbre(), curArbre = nouvArbre();
  liste chaine = nouvListe();
  char * argCmd = malloc(strlen(argv) * sizeof(char) + 1);
  strcpy(argCmd, argv);
  strcat(argCmd, "\n");
  while( t < strlen(argCmd) )
  {
      dprintf(fdHistorique, "%c", argCmd[t]);
      if(argCmd[t] == '\n' )
      {
        curArbre = addNode(';', chaine, curArbre);
        chaine = nouvListe();
        if (bascule)
        {
          arb = curArbre;
          bascule = 0;
        }
        break;
      }
      else if (argCmd[t] == '|' || argCmd[t] == ';' || argCmd[t] == '&' || argCmd[t] == '>' || argCmd[t] == '<')
      {
        curArbre = addNode(argCmd[t], chaine, curArbre);
        chaine = nouvListe();
        if (bascule)
        {
          arb = curArbre;
          bascule = 0;
        }
      }
      else
      {
        adjq(argCmd[t], chaine);
      }
      t++;
  }
  return arb;
}

int main(int argc, char** argv, char** envp)
{

  arbre arb, curArbre = nouvArbre();
  char bBuffer ;
  int bascule = 2;
  liste chaine;
  ssize_t t ;
  char cwd[1024];
  char * nomHistorique = malloc(sizeof(char)*(sizeof(HISTORY_NAME)+sizeof(getenv("HOME"))+2));
  strcpy(nomHistorique, getenv("HOME"));
  chdir(nomHistorique);
  strcat( nomHistorique, "/");
  strcat( nomHistorique, HISTORY_NAME);

  int fdHistorique = open(nomHistorique, O_RDWR | O_CREAT, 0666);
  if (fdHistorique < 0)
  {
    perror("Erreur");
    return -1;
  }

  if(argc > 2 )
  {
    dprintf(2, "Nombre d'arguments invalide");
    return 1;
  }

  if(argc == 2)
  {
    arb = createTreeFromArg(argv[1], fdHistorique);
    fdHistorique = checkhistory(fdHistorique);

    if ( isBackground(arb))
    {
      if (fork()==0)
      {
        execArbre2(arb, envp, -1, 1);
        return 0;
      }
    }
    else
    {
      execArbre2(arb, envp, -1, 1);
    }
    //vide l'arbre
    parcoursPrefixeA(arb, free_if_needed);   
    return 0;
  }

  while(1) 
  {      
    chaine = nouvListe();
    curArbre = nouvArbre(); 
    bascule = 1;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
      strcat(cwd, ":MyPrompt# \0");
      write(1, cwd, strlen(cwd));
    }
    else
    {
      write(1, "MyPrompt# \0", 10);
    }
    lseek(fdHistorique, 0, SEEK_END);
    t = read(0, &bBuffer, 1);
    while( t > 0 )
    {
      dprintf(fdHistorique, "%c", bBuffer);
      if(bBuffer == '\n' )
      {
        curArbre = addNode(';', chaine, curArbre);
        chaine = nouvListe();
        if (bascule)
        {
          arb = curArbre;
          bascule = 0;
        }
        break;
      }
      else if (bBuffer == '|' || bBuffer == ';' || bBuffer == '&' || bBuffer == '>' || bBuffer == '<')
      {
        curArbre = addNode(bBuffer, chaine, curArbre);
        chaine = nouvListe();
        if (bascule)
        {
          arb = curArbre;
          bascule = 0;
        }
      }
      else
      {
        adjq(bBuffer, chaine);
      }
      t = read(0, &bBuffer, 1);
    }

    fdHistorique = checkhistory(fdHistorique);

    if ( isBackground(arb))
    {
      if (fork()==0)
      {
        execArbre2(arb, envp, -1, 1);
        return 0;
      }
    }
    else
    {
      execArbre2(arb, envp, -1, 1);
    }
    //vide la liste
    fermeListe(chaine); 
    //vide l'arbre
    parcoursPrefixeA(arb, free_if_needed);                                                                                                                                                                                
  }

  return 0;
  
}
