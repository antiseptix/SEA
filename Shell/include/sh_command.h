#ifdef _SH_COMMAND_H
#define _SH_COMMAND_H

void sh_pwd();
void sh_exit();
void sh_cd(char **args);
void sh_echo(char **args);
void sh_help();
void execCommand(char **args);


#endif