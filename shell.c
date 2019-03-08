#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

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
	
	if(strcmp(argv[0],"exit"))
	{
		exit(EXIT_FAILURE);
	}
	else 
	if(strcmp(argv[0],"cd"))
	{
		chdir(argv[i+1]);
	}
	else
	{
		int a = fork();
		if (a == 0) //fils
			execvp(argv[i],argv);
	}
}

int parse_line_redir(char *s, char **argv[], char **in, char **out)
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
		while (s[i] == ' ')
		{
			++i;
		}
		
		if(s[i] == '<') //in
		{
			//gives input to a command
			out[0] = NULL;
			int j;
			j = i+1;
			
			while (s[j] == ' ')
			{
				++j;
			}
			
			debw = &s[j];
			wordl = 0;
			in[0] = malloc(sizeof(char*) * 1);
			
			while(s[j] && s[j] != ' ')
			{
				++wordl;
				++j;
			}
			
			if(wordl)
			{
				in[0] = malloc(sizeof(char) * wordl + 1);
				
				memcpy(in[0], debw, wordl);
				
				in[0][wordl]= '\0';
			}
		}
		else if(s[i] == '>') //out
		{
			//directs the output of a command into a file
			in[0] = NULL;
			int j;
			j = i+1;
			
			while (s[j] == ' ')
			{
				++j;
			}
			
			debw = &s[j];
			wordl = 0;
			out[0] = malloc(sizeof(char*) * 1);
			
			while(s[j] && s[j] != ' ')
			{
				++wordl;
				++j;
			}
			
			if(wordl)
			{
				out[0] = malloc(sizeof(char) * wordl + 1);
				
				memcpy(out[0], debw, wordl);
				
				out[0][wordl]= '\0';
			}
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

/*
int redir_cmd(char *argv[], char*in, char *out)
{
	//use simple_cmd NOT OK YET
	 
	
}
*/


int main(int argc, char **argv)
{
	if(argc > 1) //nom de fichier en argument
	{
		char *dir = malloc(sizeof(char) * 1024);
		
		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		int fd = open(argv[1],O_RDONLY);
		if (fd < 0)
			exit(EXIT_FAILURE);
		
		read(fd, s, 1024);
		
		parse_line(s,&tab);
		
 		simple_cmd(tab);
		
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
		
		char **in = malloc( 100 * sizeof(char*));
		char **out = malloc( 100 * sizeof(char*));
//		parse_line(s,&tab);
		parse_line_redir(s,&tab, in, out);
		if(in[0]) printf("in : %s\n",in[0]);
		if(out[0]) printf("out : %s\n",out[0]);
		
// 		simple_cmd(tab);
		printf("\n");
		affiche_cmd(tab);
		
		free(dir);
		free(tab);
		free(s);
		
// 		sigaction(); //3 == SIGQUIT
	}
	
	exit(EXIT_SUCCESS);
}