#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void affiche_cmd(char *argv[])
{
	
	unsigned int i;
	i = 0;
	
	while(argv[i])
	{
		printf("%s ",argv[i]);
		++i;
	}
	
	printf("\n");
}
	
int parse_line(char *s, char * *argv[])
{
// decoupe la chaine s terminée par \0, mots séparés dun espace dans case dif du tableau, 
// case NULL à la fin du tableau
	
	unsigned int j;
	j = 0;
	
	char *tmp = malloc(sizeof(char));
	unsigned int tmp_int;
	tmp_int = 0;
	
	while(s[i])
	{
		while(s[i] == ' ')
			++i; //nouvelle case
		
		//cpy dans case j
	
		while(s[i] != ' ') // use of strpbrk ?
		{

			tmp[tmp_int] = s[i];
			++tmp_int;
			++i;
		}
		
		++j;
		++i;
	}
	
	argv[0] = NULL; 
	
	return 0;
	
}

// char *strpbrk(const char *s, const char *accept);
// 
// DESCRIPTION
//        The strpbrk() function locates the first occurrence in the string s of any of the bytes in the string accept.
