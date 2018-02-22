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

//Création de la structure résultat - pour la recherche du min et max value du tableau simple
typedef struct
{
  int min;
  int max;
}Resultat;

typedef struct
{
    int borneStart;
    int borneEnd;
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
  arg_struct *args = (arg_struct *)arg;
  int i;
  for(i=args->borneStart; i < args->borneEnd; i++ )
  {
    if(minVal > tabCommun[i])
    {
      minVal = tabCommun[i];
    }
    if(maxVal < tabCommun[i])
    {
    	maxVal = tabCommun[i];
    }
  }
  pthread_exit(NULL);
}

void createThread(int nbThread)
{
  pthread_t tabThread[nbThread];
  arg_struct tabArgs[nbThread];

  if(nbThread<=0)
  {
    perror("On ne peut demander la création de 0 thread !");
    exit(1);
  }

  int ecart = SIZE/nbThread, reste = SIZE%nbThread;
  //si le modulo est différent de 0 il faudra rajouter le reste dans le premier ou dernier thread
  int i =0; // entier pour les itérations
  if (reste!=0)
  {
    //param particulier du premier thread qui prend en plus le resultat du modulo dans ses bornes
    tabArgs[0].borneEnd = ecart+reste;
    tabArgs[0].borneStart = 0;
    tabArgs[0].size = ecart+reste;
    pthread_create(&tabThread[0], NULL, find_min_max_thread,(void*)&tabArgs[0]);

    for(i=1;i<nbThread;i++)
    {
      //param des arguments du thread à créer
      tabArgs[i].borneEnd = ((i+1)*ecart)+reste;//ne pas invverser les bornes
      tabArgs[i].borneStart = ((tabArgs[i].borneEnd)-ecart);
      tabArgs[i].size = ecart;
      pthread_create(&tabThread[i], NULL, find_min_max_thread,(void*)&tabArgs[i]);
    }
  }else{
    for(i=0;i<nbThread;i++)
    {
      //param des arguments du thread à créer
      tabArgs[i].borneEnd = ((i+1)*ecart);//ne pas invverser les bornes
      tabArgs[i].borneStart = ((tabArgs[i].borneEnd)-ecart);
      tabArgs[i].size = ecart;
      pthread_create(&tabThread[i], NULL, find_min_max_thread,(void*)&tabArgs[i]);
    }
  }
  //on lance le join de tous les threads
  for(i = 0; i < nbThread; i++)
  {
    pthread_join(tabThread[i], NULL);
  }
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

  printf("Avant : %ld us\tApres : %ld us\n",temps_avant.tv_usec,temps_apres.tv_usec);
  printf("Avant : %ld s\tApres : %ld s\n",temps_avant.tv_sec,temps_apres.tv_sec);
  printf("MIN : %d\n", result.min);
  printf("MAX : %d\n", result.max);
  //printf("Temps de recherche : %ld us \n", (temps_apres.tv_usec-temps_avant.tv_usec)); // n'est pas stable si durée trop longue
  printf("Temps de recherche : %ld us\n\n",((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);


  //----- test fonc thread ------
  printf("###METHODE THREAD###\n");
  //arg_struct argTest; // struc contenant le pointeur du tbl et la taille du tbl

  //argTest.tab = tabCommun;// le pointeur tableau dans argTest prend la valeur de notre pointeur.
  //argTest.size = SIZE;
  //pthread_t threadTri; // variable représentant le thread

  int nbThread = 0;
  printf("Veuillez entrer ne nombre de thread voulu :");
  scanf("%d",&nbThread);
  gettimeofday(&temps_avant, NULL);
  // 1 er argurment = pointeur vers id du thread (un pthread_t)
  // 2nd argument les attributs du thread (joignable par défaut) on utilise souvent NULL
  //3e argument un pointeur vers la fonction à executer dans le thread de forme void * fonc(void* arg)
  //4e argument : est l'argument à passer au thread
  /**if(pthread_create(&threadTri, NULL, find_min_max_thread,(void*)&argTest) == -1) {
   perror("pthread_create");
   return EXIT_FAILURE;
  }
  printf("Thread crée , instruction join prochainement\n");
  //Une fois un thread lancé il va faloir donnerl'instruction d'attendre la fin de celui-ci car sinon le thread principal continue et termine son execution avant.
  if(pthread_join(threadTri, NULL)){
    perror("pthread_join");
    return EXIT_FAILURE;
  }// join = attente la fin du thread **/


  createThread(nbThread);

  gettimeofday(&temps_apres, NULL);
  printf("Avant : %ld us\tApres : %ld us\n",temps_avant.tv_usec,temps_apres.tv_usec);
  printf("Avant : %ld s\tApres : %ld s\n",temps_avant.tv_sec,temps_apres.tv_sec);
  printf("min value :%d\tmax value :%d\n",minVal,maxVal);
  printf("Temps de recherche : %ld us\n\n",((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);



  return EXIT_SUCCESS;
}
