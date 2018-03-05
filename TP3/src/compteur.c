#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

static int cont = 1;
static int nb = 0;

void handlerSIGINT()
{
	cont = 0;
	printf("\nLe programme à été interrompu. Dernière valeur: %d\n", nb);
}

void handlerSIGTERM()
{
	cont = 0;
	printf("Fin du programme\n");
}

int main ()
{
	int c=0;

	// Intercepte les signaux d'interrupteurs et de fin
	signal(SIGINT, handlerSIGINT);
	signal(SIGTERM, handlerSIGTERM);

	printf("Le programme tourne en boucle,  CTRL + C pour l'arreter\n");

	while (cont == 1)
	{
		// Compteur incrémental
	   	nb++;
	    if ((nb % 100000000) == 0)
	    {
	    	c++;
	    	printf("%d\n", nb);
	    	fflush(stdout);
	    }
	}
	return 0;
}
