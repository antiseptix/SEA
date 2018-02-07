#ifndef _BUILTIN_H
#define _BUILTIN_H

#define NB_LINE_HISTORY 4
#define HISTORY_NAME "MySHELL_Historique.txt"

#define TRUE 1
#define FALSE 0
typedef int bool;

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_pwd(char ** args);
int lsh_echo(char ** args);
int lsh_history(char ** args);
int execBuiltin(char * arg);
int lsh_num_builtins();



#endif