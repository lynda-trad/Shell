#ifndef __SHELL_H__
#define __SHELL_H__

void affiche_cmd(char *argv[]);
int parse_line(char *s, char * *argv[]);

#endif
