#ifdef _SH_COMMAND_H
#define _SH_COMMAND_H

int sh_pwd();
int sh_exit();
int sh_cd(char **args);
int sh_echo(char **args);
int sh_help();
void execCommand(char **args);


#endif