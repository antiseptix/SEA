#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
//TP-3-b : processus et thread
//initialisation du grand tbl d'entier (pas très grand pour les tests)
//Pour changer la taille du tableau
#define SIZE (int) 1000
//#define SIZE (int) 1e8

//Création de la structure résultat - pour la recherche du min et max value du tableau
typedef struct
{
  int min;
  int max;
}Resultat;

typedef struct
{
    int * tab;
    int size;
}arg_struct;

int maxVal = 0x80000000;
int minVal = 0x7FFFFFFF;

//initialisation tu tableau avec des entiers aléatoires
void initializeTab(int *tab)
{
  int i;
  time_t t;
  srand((unsigned) time(&t)); // initialisation de rand
  for(i=0; i < SIZE; i++ )
  {
    // Création des nombres au random
    tab[i] = rand()%9999999;
    //printf("%d\n", tab[i]); //Pour afficher les chiffre générés
  }
  tab[0] = 171;
  printf("---Tableau initialisé--- \n");
}

int find_min_max(int tab[], Resultat* result) // recherche OK
{
  //init des values min et max
  result->max=tab[0];
  result->min=tab[0];
  int i;
  for(i=0; i < SIZE; i++ )
  {
    if(result->max<tab[i])
    {
      result->max=tab[i];
    }
    if(result->min>tab[i])
    {
      result->min=tab[i];
    }
  }
  return result;
}

void *find_min_max_thread(void *arg)
{

  arg_struct *args = (arg_struct *)arg;
  int i;
  for(i=0; i < args->size; i++ )
  {
    if(minVal > args->tab[i])
    {
      minVal = args->tab[i];
    }
    if(maxVal < args->tab[i])
    {
    	maxVal = args->tab[i];
    }
  }

  pthread_exit(NULL);
}

void createThread(int nbThread, int *tab)
{
  pthread_t tabThread[nbThread];
  //Vérification que le nombre de thread est suppérieur à 0
  if(nbThread<=0)
    exit(1);

  int size = SIZE/nbThread, reste = SIZE%nbThread;
  if (reste!=0)
  {

  }

  //  for(i=0; i<nbThread; i++)
  //  {
  //    pthread_create(&tabThread[i], NULL, &find_min_max);
  //  }


}

void *thread_1(void *arg)
{
  printf("Ola c'est l'interieur d'un thread ici !\n");

  (void) arg;
  pthread_exit(NULL);
}


int main(int argc, char** argv)
{
  // Instantiation de la structure résultat
  Resultat result;

  int *tab = (int *)malloc(SIZE*sizeof(int)); //Instantiation du tableau
  initializeTab(tab);

  //structure timeval existante avec l'include de time
  struct timeval temps_avant, temps_apres;
  //Récupération du temps avant
  gettimeofday(&temps_avant, NULL);
  //traitement tableau
  find_min_max(tab, &result);
  //Récupération du temps après
  gettimeofday(&temps_apres, NULL);

  printf("Avant : %ld us\n",temps_avant.tv_usec);
  printf("Apres : %ld us\n",temps_apres.tv_usec);

  printf("MIN : %d\n", result.min);
  printf("MAX : %d\n", result.max);
  printf("Temps de recherche : %ld us \n", (temps_apres.tv_usec-temps_avant.tv_usec));


  // TEST DE CREATION DE THREAD
  /**
  pthread_t thread; // variable représentant le thread

  printf("Je s'appelle THREAD \n");
  // 1 er argurment = pointeur vers id du thread (un pthread_t)
  // 2nd argument les attributs du thread (joignable par défaut) on utilise souvent NULL
  //3e argument un pointeur vers la fonction à executer dans le thread de forme void * fonc(void* arg)
  //4e argument : est l'argument à passer au thread
  if(pthread_create(&thread, NULL, thread_1,(void*)NULL) == -1) {
   perror("pthread_create");
   return EXIT_FAILURE;
  }

  //Une fois un thread lancé il va faloir donnerl'instruction d'attendre la fin de celui-ci car sinon le thread principal continue et termine son execution avant.
  if(pthread_join(thread, NULL)){
    perror("pthread_join");
    return EXIT_FAILURE;
  }//join = attendre la fin du thread

  //void pthread_exit(void *ret); // pour supprimer un thread à la fin de l'utilisation
  printf("NOUS SOMMES PU THREAD \n"); **/

  //----- test fonc thread ------

  printf("min value :%d\tmax value :%d\n",minVal,maxVal);
  arg_struct argTest;

  int *tabTest = (int *)malloc(SIZE*sizeof(int)); //Instantiation du tableau
  initializeTab(tabTest);
  argTest.tab = tabTest;

  printf("crétation du thread pour le tableau\n");
  printf("%d\n", argTest);
  printf("%d\n", &argTest);
  printf("%d\n",argTest.tab[0] );

  pthread_t threadTri;/**
  if(pthread_create(&threadTri, NULL, find_min_max_thread,(void*)&argTest) == -1) {
   perror("pthread_create");
   return EXIT_FAILURE;
  }
  printf("thread crée , instruction join prochainement");
  if(pthread_join(threadTri, NULL)){
    perror("pthread_join");
    return EXIT_FAILURE;
  }

  printf("min value :%d\tmax value :%d\n",minVal,maxVal);
**/
  return EXIT_SUCCESS;
}
