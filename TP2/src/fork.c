#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	int f;
	int status;
	f = fork();
	if(f==0){

		// fils
		int pid = getpid();
		int ppid = getppid();
		printf("----FILS-----\n");
		printf("PID Fils : %d\n", pid);
		printf("PID Père : %d\n", ppid);

		// Renvoi le dernier chiffre du pid au père
		int reste = pid % 10;
		exit(reste);
	}
	else{

		// père
		printf("----PERE-----\n");
		int pid = getpid();
		printf("PID Père : %d\n", pid);

		// Attend le fils
		wait(&status);
		
		// Récupère le dernier chiffre du pid fils
		printf("Code retour fils (depuis père): %d\n", WEXITSTATUS(status));
	}

}
