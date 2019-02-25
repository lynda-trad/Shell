#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


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

void simple_cmd(char *argv[])
{
	unsigned int i;
	i = 0;
	
	while(argv[i])
	{
		if(strcmp(argv[i],"exit"))
		{
			exit(EXIT_FAILURE);
		}
		else if(strcmp(argv[i],"cd"))
		{
// 			int status;
			chdir(argv[i+1]);
// 			waitpid(-1,&status,0);
		}
		++i;
	}
}



int main()//int argc, char **argv
{
	
	while(1) //exit avec CTRL+C et pas CTRL+D -> how ?
	{
		char *dir = malloc(sizeof(char) * 1024);
		
		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		printf("%s$",getcwd(dir,1024));
		fgets(s,1024,stdin);
		
		parse_line(s,&tab);
		
		simple_cmd(tab);
		affiche_cmd(tab);
		
		free(dir);
		free(tab);
		free(s);
	}
	
	exit(EXIT_SUCCESS);
	
}