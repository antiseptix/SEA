#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

//TP-3-b : processus et thread

//variables globales Le tableau commun et le min max values pour les threads.
#define SIZE (int) 1e8 //#define SIZE (int) 1e8
int *tabCommun;
int maxVal = 0x80000000;
int minVal = 0x7FFFFFFF;

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



//initialisation tu tableau avec des entiers aléatoires !!(on garde le param tab pour l'utiliser sur d'autre tbl si besoin)
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
  printf("---Tableau initialisé--- \n");
}

void find_min_max(Resultat* result) // recherche OK
{
  //init des values min et max
  result->max=tabCommun[0];
  result->min=tabCommun[0];
  int i;
  for(i=0; i < SIZE; i++ )
  {
    if(result->max<tabCommun[i])
    {
      result->max=tabCommun[i];
    }
    if(result->min>tabCommun[i])
    {
      result->min=tabCommun[i];
    }
  }
}

void *find_min_max_thread(void *arg)
{
	printf("INSIDE FONC DE TRI THREAD\n");
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
  printf("FIN DU THREAD\n");
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


int main(int argc, char** argv)
{
	tabCommun = (int *)malloc(SIZE*sizeof(int)); //Instantiation du tableau commun
	initializeTab(tabCommun); // init du tbl en variable commune avec l'attribution des valeures random.

	//------test fonction basique de tri------------

  // Instantiation de la structure résultat
  Resultat result;
  //structure timeval existante avec l'include de time
  struct timeval temps_avant, temps_apres;
  //Récupération du temps avant
  gettimeofday(&temps_avant, NULL);
  //traitement tableau
  find_min_max(&result);
  //Récupération du temps après
  gettimeofday(&temps_apres, NULL);

  printf("Avant : %ld us\n",temps_avant.tv_usec);
  printf("Apres : %ld us\n",temps_apres.tv_usec);
  printf("Avant : %ld us\n",temps_avant.tv_sec);
  printf("Apres : %ld us\n",temps_apres.tv_sec);

  printf("MIN : %d\n", result.min);
  printf("MAX : %d\n", result.max);
  printf("Temps de recherche : %ld us \n", (temps_apres.tv_usec-temps_avant.tv_usec));
  printf("\n---- Temps de recherche : %ld us\n\n",((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);


  //----- test fonc thread ------
  printf("###METHODE THREAD###\n");
  printf("min value :%d\tmax value :%d\n",minVal,maxVal);
  arg_struct argTest; // struc contenant le pointeur du tbl et la taille du tbl

  argTest.tab = tabCommun;// le pointeur tableau dans argTest prend la valeur de notre pointeur.
  argTest.size = SIZE;

  printf("Crétation du thread pour le tableau\n");

  pthread_t threadTri; // variable représentant le thread

  gettimeofday(&temps_avant, NULL);
  // 1 er argurment = pointeur vers id du thread (un pthread_t)
  // 2nd argument les attributs du thread (joignable par défaut) on utilise souvent NULL
  //3e argument un pointeur vers la fonction à executer dans le thread de forme void * fonc(void* arg)
  //4e argument : est l'argument à passer au thread
  if(pthread_create(&threadTri, NULL, find_min_max_thread,(void*)&argTest) == -1) {
   perror("pthread_create");
   return EXIT_FAILURE;
  }
  printf("Thread crée , instruction join prochainement\n");
  //Une fois un thread lancé il va faloir donnerl'instruction d'attendre la fin de celui-ci car sinon le thread principal continue et termine son execution avant.
  if(pthread_join(threadTri, NULL)){
    perror("pthread_join");
    return EXIT_FAILURE;
  }// join = attente la fin du thread
  gettimeofday(&temps_apres, NULL);  
  printf("Avant : %ld us\n",temps_avant.tv_usec);
  printf("Apres : %ld us\n",temps_apres.tv_usec);
  printf("Avant : %ld us\n",temps_avant.tv_sec);
  printf("Apres : %ld us\n",temps_apres.tv_sec);
  printf("Temps de recherche : %ld us \n", (temps_apres.tv_usec-temps_avant.tv_usec));
  printf("min value :%d\tmax value :%d\n",minVal,maxVal);
  printf("\n---- Temps de recherche : %ld us\n\n",((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);

  return EXIT_SUCCESS;
}
