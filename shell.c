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

int parse_line(char *s, char **argv[])
{
	unsigned int i;
	unsigned int len;
	unsigned int wordl;
	char **tmp;
	char *debw;
	
	i = 0;
	len = 0;
	tmp = malloc(sizeof(char*) * 1);
	
	while(s[i])
	{
		while (s[i] ==' ')
		{
			++i;
		}
		debw = &s[i];
		wordl = 0;
		
		while(s[i] && s[i] != ' ')
		{
			++wordl;
			++i;
		}
		
		if(wordl)
		{
			++len;
			
			tmp = realloc(tmp, sizeof(char*) * (len + 1));
			
			tmp[len-1] = malloc(sizeof(char) * wordl + 1);
			
			memcpy(tmp[len-1], debw, wordl);
			
			tmp[len-1][wordl]= '\0';
			
		}
		
	}
	
	tmp[len] = NULL;
	argv[0] = tmp;
	
	return len;
}





int main(){//int argc, char **argv
	printf("\n------ Affiche_cmd\n");
	
	char **tab = malloc( 100 * sizeof(char*));
	
	char *e = "commande -v toto  tata";
	char *s = malloc(1024 * sizeof(char));
	
 	parse_line(s,&tab);
	affiche_cmd(tab);

	exit(EXIT_SUCCESS);
	
}