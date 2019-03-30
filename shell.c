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
	
	while(s[i] && s[i] != '\n')
	{
		while (s[i] ==' ')
		{
			++i;
		}
		
		debw = &s[i];
		wordl = 0;
		
		while(s[i] && s[i] != ' ' && s[i] != '\n')
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
// 	printf("%s",argv[0]); //il y a un \n a la fin du argv[0] 
	if(!*argv)
		return;
	if(!strcmp(argv[0],"exit"))
	{
		exit(EXIT_FAILURE);
	}
	else 
	if(!strcmp(argv[0],"cd"))
	{
		if(argv[1])
			chdir(argv[1]);
	}
	else
	{
		pid_t p = fork();
		if(p == 0)
		{
			execvp(argv[0],argv);
		}
	}
}

int parse_line_redir(char *s, char **argv[], char **in, char **out)
{
	unsigned int i;
	unsigned int len;
	unsigned int wordl; //length de chaque mot dans la commande
	char **tmp; 		// argv
	char *debw; 		// debut du mot quon va memcpy
	i = 0;
	len = 0;
	tmp = malloc(sizeof(char*) * 1);
	
	while(s[i] && s[i] != '\n')
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
		
		while(s[i] && s[i] != ' ' && s[i] != '\n')
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
	
	return i;
}


int redir_cmd(char *argv[], char*in, char *out)
{
	simple_cmd(argv); 
	
	int fd = open(argv[0],O_RDONLY);
	
	if(out) // write only dans le out 
	{ 
		int a = fork();
		if (a == 0)
		{
			int fout = open(out,O_WRONLY);
			dup2(fout,STDOUT_FILENO);
			execvp(argv[0],argv);
		}
	}
	else
	if(in) // read only dans le in
	{  
		int a = fork();
		if (a == 0)
		{
			int fin = open(in,O_RDONLY);
			dup2(fin,STDIN_FILENO);
			execvp(argv[0],argv);
		}
	}
	return fd;
}

int parse_line_pipes(char *s, char **argv[], char **in, char **out)
{
// slice s en sous tableaux contenant chacun le tableau d'arguments de la commande : command toto | commend2 tata | command3
// pipe connects the standard output of the first command to the standard input of the second command
	
	//malloc grand tableau puis malloc chaque case pour chaque commande
	unsigned int i; //case de la chaine s
	unsigned int len; //length du tableau
	i = 0;
	len = 0;
	
	while(s[i])
	{
		i = parse_line_redir(&s[i], argv+len, in, out); // on fait comme parseline redir pour chaque case len du tableau argv
		
		while (s[i] == ' ')
		{
			++i;
		}
		
		if(s[i] == '|') //pipe
		{
			++len;
		}
	}
	return i;
}


/*
	{
	{
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
	}
	}
*/


//pour pipe
int redir_cmd_pipe(char *argv[], char*in, char *out)
{
	simple_cmd(argv); 
	
	int fd = open(argv[0],O_RDONLY);
	
	if(out && in) // write only dans le out 
	{ 
		int fin = open(in,O_RDONLY);
		int fout = open(out,O_WRONLY);
		int fildes[2];
		fildes[0] = fin;
		fildes[1] = fout;
		pipe(fildes);
		execvp(argv[0],argv);
	}
	return fd;
}


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
		
		printf("%s$ ",getcwd(dir,1024));
		affiche_cmd(tab);
		
		read(fd, s, 1024);
		
		parse_line(s,&tab);
		
		simple_cmd(tab);
		
		free(dir);
		free(tab);
		free(s);
	}
	else while(1)
	{
		char *dir = malloc(sizeof(char) * 1024);
		
		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		printf("%s$ ",getcwd(dir,1024));
		
		char *ex;
		ex = fgets(s,1024,stdin);
		
		if(ex == 0) //Quit with CTRL+D
		{
			printf("\n");
			break;
		}
		
		parse_line(s,&tab);
		affiche_cmd(tab);
		
		simple_cmd(tab);
		
		/*
		char **in = malloc( 100 * sizeof(char*));
		char **out = malloc( 100 * sizeof(char*));
		parse_line_redir(s,&tab, in, out);
		if(in[0]) printf("in : %s\n",in[0]);
		if(out[0]) printf("out : %s\n",out[0]);
		*/
		
		free(dir);
		free(tab);
		free(s);
	}
	exit(EXIT_SUCCESS);
}