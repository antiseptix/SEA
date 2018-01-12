#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE (int) 10

typedef struct
{
  int min;
  int max;
}Resultat;


void initializeTab(int *tab)
{
  int i;
    time_t t;
  srand((unsigned) time(&t)); // initialisation de rand
  for(i=0; i < SIZE; i++ )
  {
    tab[i] = rand()%9999999;
    printf("%d\n", tab[i]);
  }
  printf("---Tableau initialisé--- \n");
}


int find_min_max(int tab[], Resultat* result)
{
  //result->max=tab[0];
  //result->min=tab[0];
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

void createThread(int nbThread, int *tab)
{
  pthread_t tabThread[nbThread];

  if(nbThread<=0)
    exit(1);

  int size = SIZE/nbThread;

  for(i=0; i<nbThread; i++)
  {
    pthread_create(&tabThread[i], NULL, &find_min_max);
  }


}


int main(int argc, char** argv)
{
  Resultat result;
  result.min=100000000000;
  int *tab = (int *)malloc(SIZE*sizeof(int));
  initializeTab(tab);

  struct timeval temps_avant, temps_apres;
  gettimeofday(&temps_avant, NULL);

  find_min_max(tab, &result);

  gettimeofday(&temps_apres, NULL);

  printf("Avant : %d us\n",temps_avant);
  printf("Apres : %d us\n",temps_apres);

  printf("MIN : %d\n", result.min);
  printf("MAX : %d\n", result.max);
  printf("Temps de recherche : %ld us \n", (temps_apres.tv_usec-temps_avant.tv_usec));
  //printf("MAX : %d\n", find_max(tab));

  //pthread_t thread;
  //pthread_create(&thread, NULL, yolo, NULL);
  //pthread_join(thread, NULL);

  //pthread_join(thread, NULL);
  printf("NOUS SOMMES PU THREAD \n");

}
