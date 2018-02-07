#ifndef _STRINGGOODIES_H
#define _STRINGGOODIES_H


#define TRUE 1
#define FALSE 0

typedef int bool;

char *trimwhitespace(char *str);
char *str_replace(char *orig, char *rep, char *with);
char** str_split(char* a_str, const char a_delim);
bool startsWith(const char *pre, const char *str); 

#endif
