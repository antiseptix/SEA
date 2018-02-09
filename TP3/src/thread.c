#include <stdio.h>
#include <stdlib.h>
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

  pthread_t thread;
  //pthread_create(&thread, NULL, nomDeFonctionQueTulanceDAnsLeThread, NULL);
  //pthread_join(thread, NULL); //join = attendre la fin du thread

  //pthread_join(thread, NULL);

  //printf("NOUS SOMMES PU THREAD \n");

}
