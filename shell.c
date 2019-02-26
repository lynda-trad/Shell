#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


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
			tmp[len] = malloc(sizeof(char) * wordl + 1);
			
			memcpy(tmp[len], debw, wordl);
			
			tmp[len][wordl]= '\0';
			
			++len;
			
			tmp = realloc(tmp, sizeof(char*) * (len + 1));
		}
	}
	
	tmp[len] = NULL;
	argv[0] = tmp;
	
	return len;
}

void simple_cmd(char *argv[])  // NOT OK
{
	unsigned int i;
	i = 0;
	
	while(argv[i])
	{
		if(strcmp(argv[i],"exit"))
		{
			exit(EXIT_FAILURE);
		}
		else 
		if(strcmp(argv[i],"cd"))
		{
			chdir(argv[i+1]);
			printf("%d",i); // 0134 2 = "cd"
		}
		++i;
	}
	
}

/*
int parse_line_redir(char *s, char **argv[], char **in, char **out)
{
	//analyse chaine cm parse_line mais prend en compte redirections < en entree et > en sortie 
	//nom de fichier en entree et sortie stockés dans in et out respectivement
	//variable correspondante a null si une redirection n'a pas lieu
	
}*/

/*
int redir_cmd(char *argv[], char*in, char *out)
{
	//use simple_cmd NOT OK YET
	 
	
}
*/


int main(int argc, char **argv)
{
	if(argc >1)
	{
		char *dir = malloc(sizeof(char) * 1024);
		
		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		int fd = open(argv[1],O_RDONLY);
		if (fd < 0)
			exit(EXIT_FAILURE);
		
		read(fd, s, 1024);
		
		parse_line(s,&tab);
		
// 		simple_cmd(tab);
		
		printf("%s$",getcwd(dir,1024));
		affiche_cmd(tab);
		
		free(dir);
		free(tab);
		free(s);
	}
	else while(1) //exit avec CTRL+C et pas CTRL+D -> how ?
	{
		char *dir = malloc(sizeof(char) * 1024);
		
		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		printf("%s$",getcwd(dir,1024));
		
		fgets(s,1024,stdin);
		
		parse_line(s,&tab);
		
// 		simple_cmd(tab);
		
		affiche_cmd(tab);
		
		free(dir);
		free(tab);
		free(s);
	}
	
	exit(EXIT_SUCCESS);
	
}