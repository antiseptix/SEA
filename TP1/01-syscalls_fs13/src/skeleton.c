/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <linux/limits.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <getopt.h>


#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS]"
#define USAGE_PARAMS "OPTIONS:\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);
    
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hvi:o:";





//nouvelle methode ajouté pour le tp1 fontion 3
int list_dir(const char *dirname)     {

	struct dirent* current_directory;
	struct stat my_stat;
	struct tm lt;  
	struct passwd *pwd; // For User-ID
	struct passwd *pwd_g; // For Group-ID

	DIR* directory = opendir(dirname); //ouverture du dossier

    if(directory == NULL)     { // ernno si ouverture du dossier impossible 

    printf("list_dir : %s : %s \n", dirname, strerror(errno));

    return 0;
	}   

    printf("\nDirectory : %s\n", dirname);
    printf("\n");

    while( (current_directory = readdir(directory) ) )     {
    	printf("\n     --------------     \n");

	    stat(current_directory->d_name, &my_stat);  // nom de l'elem

	    if ( (stat(current_directory->d_name, &my_stat) ) == 0 )    {
	        pwd = getpwuid(my_stat.st_uid); // Get User-ID
	        pwd_g = getpwuid(my_stat.st_gid); // Get Group-ID
	    }

	    //nom du fichier
	    printf("%s",current_directory->d_name);
    
		//Permission		
		printf("\t");
		printf( (S_ISDIR(my_stat.st_mode)) ? "d" : "-");
		printf( (my_stat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (my_stat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (my_stat.st_mode & S_IXUSR) ? "x" : "-");
		printf( (my_stat.st_mode & S_IRGRP) ? "r" : "-");
		printf( (my_stat.st_mode & S_IWGRP) ? "w" : "-");
		printf( (my_stat.st_mode & S_IXGRP) ? "x" : "-");
		printf( (my_stat.st_mode & S_IROTH) ? "r" : "-");
		printf( (my_stat.st_mode & S_IWOTH) ? "w" : "-");
		printf( (my_stat.st_mode & S_IXOTH) ? "x" : "-");
		printf("\n");

		//le propriétaire
		printf("propriétaire: %s \t groupe: %s", pwd->pw_name,pwd_g->pw_name);
        printf("\n");

        //taille fichier - size 
        printf("taille: %ld \n",(long)my_stat.st_size);

        // Last Modified
        time_t t = my_stat.st_mtime;
        localtime_r(&t, &lt);
        char timebuf[80];
        //strftime(timebuf, sizeof(timebuf), "%c", &lt);
        strftime(timebuf, sizeof(timebuf),"%d%m%Y @ %H:%M", &lt); // mettre ici le bon format pour le buffer. OK fait !
        printf("%s \n", timebuf );

        


		//################ surplus de la fonc 
        if (pwd != 0) {
	        printf("%s \t %ld \t %s \t %s", pwd->pw_name, (long)my_stat.st_size, timebuf, current_directory->d_name);
	        printf("\n");
        } else  { // si on ne peut pas avoir le nom du prop on affiche son user id UID à la place
            printf("%d \t %ld \t %s \t %s", my_stat.st_uid, (long)my_stat.st_size, timebuf, current_directory->d_name);
            printf("\n");
        }

	}
    closedir(directory);
    return 0; 
}


/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);         
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  // Printing params
  dprintf(1, "** PARAMS **\n %-8s: %d",
          "verbose", is_verbose_mode);

  // Business logic must be implemented at this point
	
  /* LOREM IPSUM DOT SIR AMET */
  
  errno = 0; // on réinitialise la variable errno
  
  //https://stackoverflow.com/questions/29993653/linux-ls-al-like-program-in-c ######################################################################"" a check
  //https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c #### pour les permission
  //avec structure stat => permet d'obtenit l'état d'un fichier.

  	printf("\nInformations des fichiers & répertoires présents :\n");
	
	char *curr_dir = NULL; 
	DIR *dp = NULL; 
	struct dirent *dptr = NULL; 
	unsigned int count = 0; 

	// Get the value of environment variable PWD 
	curr_dir = getenv("PWD");
	if(NULL == curr_dir) 
	{ 
		printf("\n ERROR : Could not get the working directory\n");
		//lacher un ernno ici
		return -1; 
	} 

	// Open the current directory 
	dp = opendir((const char*)curr_dir); 
	if(NULL == dp)
	{ 
		printf("\n ERROR : Could not open the working directory\n"); 
		//lacher un ernno ici
		return -1;
	} 

	//-------------------- en dessous printer les infos
	printf("\n");
	
	// Go through and display all the names (files or folders) 
	// Contained in the directory. 
	for(count = 0; NULL != (dptr = readdir(dp)); count++) 
	{ 
		printf(" %s ",dptr->d_name);
	} 
	printf("\n nombre d'élements :  %u \n", count);

	//passer en param le chemin à check(le rajouter dans les param si besoin mais ici on prend le repertoire racine à la console)
	return list_dir ( "." );


	//-----------------

	//gestion des erreurs (utilisation de strerror())
	if(errno != 0)
	{
	  fprintf(stderr,"Erreur : %s\n",strerror(errno));
	  return EXIT_FAILURE;
	}
  


  return EXIT_SUCCESS;
}
