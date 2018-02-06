#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int c = 1;
	char in[40] = "";


	while (c==1) {
		char path[40] = "";
		getcwd(path, sizeof(path));
		//system("pwd");
		printf("\033[34m %s :", path);

		fgets(in, sizeof(in), stdin);

		printf("\033[37m %s\n", in);
	}


}
