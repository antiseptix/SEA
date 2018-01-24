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

#include <getopt.h>


#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS] -i INPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
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
  { "input",   required_argument, 0, 'i' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hvi:o:";



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
  char* bin_input_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'i':
        //input param
        if (optarg)
        {
          bin_input_param = dup_optarg_str();         
        }
        break;
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
         
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  if (bin_input_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_input_param);
    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
  }


  // Printing params
  dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %d\n", 
          "input",   bin_input_param,
          "verbose", is_verbose_mode);

  // Business logic must be implemented at this point
	
  /* LOREM IPSUM DOT SIR AMET */
  
  errno = 0; // on réinitialise la variable errno
  
  int descInput; // input
  
  
  //ouverture descripteur lecture input file
  descInput = open(bin_input_param,O_RDONLY);
  
   //gestion des erreurs de mauvais fichiers d'entrée (utilisation de strerror())
  if(errno != 0)
  {
	  fprintf(stderr,"Erreur ficheir d'entrée : %s\n",strerror(errno));
	  return EXIT_FAILURE;
  }
  
	//char à print en sortie
	char c;

	// recupere le nb de caractère du fichier dans offset et on se place à la fin.
	//lseek(int fileDescriptor,off_t offset, int whence);
	off_t offset = lseek(descInput, 0, SEEK_END);

	while(offset >=0)
	{
		read(descInput,&c,1);
		
		dprintf(STDOUT,"%c",c);
		
		//on recule de -2 et on décremente la valeure de l'offset (vue comme taille restante)
		offset = lseek(descInput, -2, SEEK_CUR);
	}
	dprintf(STDOUT,"\n"); 
    
 
  
  //il faut fermer des descripteurs 
  close(descInput);  
  // Freeing allocated data
  free_if_needed(bin_input_param);


  return EXIT_SUCCESS;
}
/*
int reverse(char* bin_input_param){

  // regarde si le fichier est ouvert avec errno
  int input = open(bin_input_param, O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
  //avec open la tete de lecture se trouve sur l'octet 0
  //read /write avance la tete de lecture 
  //on peut la positioner avec lseek()
  
  int i=0;

  if (input >= 0 ) {
	
	//char à print en sortie
	char c;

    // recupere le nb de caractère du fichier dans offset et on se place à la fin.
    //lseek(int fileDescriptor,off_t offset, int whence);
    off_t offset = lseek(input, 0, SEEK_END);
    
    while(offset >0)
    {
		read(input,&c,offset);
		
		dprintf(STDOUT,"%c",c);
		
		/on recule de -2
		offset = lseek(input, -2, SEEK_CUR);
	}
	dprintf(STDOUT,"\n"); 
    
    
    //on replace la tete de lecture au debut.
    lseek(input, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(input_size);
    read(input, buffer, input_size);

    for(i=input_size; i>=0; i--){
      dprintf(STDOUT,"%c", buffer[i]);

    }
    dprintf(STDOUT,"\n");  
    
  }
  close(input);

}
*/
