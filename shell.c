#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define BUFF_SIZE 100
//pour les pipes

static void handler()
{
	//ignore the signal
}

void affiche_cmd(char *argv[])
{
	unsigned int i;
	i = 0;
	
	while(argv[i])
	{
		printf("%s",argv[i]);
		++i;
	}
	printf("\n");
}

void affiche_cmd_pipe(char **str[])
{
	unsigned int i;
	
	i = 0;
	while (str[i])
	{
		affiche_cmd(str[i]);
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

void simple_cmd(char *argv[])
{
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
		int status;
		if(p == 0)
		{
			execvp(argv[0],argv);
		}
		else
			wait(&status);
	}
}

int parse_in_out(char *s, int i, char **in, char **out)
{
	
	char *debw;
	unsigned int wordl;
	
	wordl = 0;
	
	if(s[i] == '<') //in
	{ //gives input to a command
		++i;
		
		while (s[i] == ' ')
		{
			++i;
		}
		
		debw = &s[i];
		wordl = 0;
		in[0] = malloc(sizeof(char*) * 1);
		
		while(s[i] && s[i] != ' ' && s[i] != '\n')
		{
			++wordl;
			++i;
		}
		
		if(wordl)
		{
			in[0] = malloc(sizeof(char) * wordl + 1);
			
			memcpy(in[0], debw, wordl);
			
			in[0][wordl]= '\0';
		}
	}
	else if(s[i] == '>') //out
	{ //directs the output of a command into a file
		++i;
		
		while (s[i] == ' ')
		{
			++i;
		}
		
		debw = &s[i];
		wordl = 0;
		out[0] = malloc(sizeof(char*) * 1);
		
		while(s[i] && s[i] != ' ' && s[i] != '\n')
		{
			++wordl;
			++i;
		}
		
		if(wordl)
		{
			out[0] = malloc(sizeof(char) * wordl + 1);
			
			memcpy(out[0], debw, wordl);
			
			out[0][wordl]= '\0';
		}
	}
	return i;
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
	
	in[0] = NULL;
	out[0] = NULL;
	
	while(s[i] && s[i] != '\n')
	{
		while (s[i] == ' ')
		{
			++i;
		}
		
		if(s[i] == '<')
		{
			i = (unsigned int) parse_in_out(s,i,in,out);
		}
		
		if(s[i] == '>')
		{
			i = (unsigned int) parse_in_out(s,i,in,out);
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


int redir_cmd(char *argv[], char *in, char *out)
{
	if(in && out)
	{
		pid_t p = fork();
		int status;
		if (p == 0)
		{
			int fin = open(in,O_RDONLY);
			int fout = open(out,O_WRONLY|O_TRUNC);
			
			dup2(fin,STDIN_FILENO);
			dup2(fout,STDOUT_FILENO);
			execvp(argv[0],argv);
			close(fin);
			close(fout);
		}
		else
			wait(&status);
	}
	else	if(out) // write only dans le out 
	{ 
		pid_t p = fork();
		int status;
		if (p == 0)
		{
			int fout = open(out,O_WRONLY|O_TRUNC);
			dup2(fout,STDOUT_FILENO);
			execvp(argv[0],argv);
			close(fout);
		}
		else
			wait(&status);
	}
	else if(in) // read only dans le in
	{  
		pid_t p = fork();
		int status;
		if (p == 0)
		{
			int fin = open(in,O_RDONLY);
			dup2(fin,STDIN_FILENO);
			execvp(argv[0],argv);
			close(fin);
		}
		else
			wait(&status);
	}
	return 0;
}


unsigned int skip_space(const char *s, unsigned int i)
{
	while (s[i] == ' ' || s[i] == '\t')
	{
		++i;
	}
	return i;
}

int parse_line_pipes(char *s, char ***argv[], char **in, char **out)
{
	unsigned int i;	//case de la chaine s
	unsigned int len; 	//length du tableau
	char *unused; 		//in possible que pour premiere commande 
	
	len = 1;
	argv[0] = malloc(sizeof(char **) * 2);
	i = skip_space(s, 0);
	i += (unsigned int) parse_line_redir(&s[i], argv[0], in, out); // += et pas juste =
	i = skip_space(s, i);
	
	while(s[i] == '|')
	{
		++i;
		argv[0] = realloc(argv[0], sizeof(char **) * (len + 2));
		i = skip_space(s, i);
		i += (unsigned int) parse_line_redir(&s[i], &argv[0][len], &unused, out);
		i = skip_space(s, i);
		++len;
	}
	
	argv[0][len] = NULL;
	return i;
}

// pipe connects the standard output of the first command to the standard input of the second command

/*	command 1 | command 2
 *	
 *	équivaut a
 * 
 *	command 1 > command 2
 *	command 2 < command 1
 */

//tests : ls -l|pipe  //  cat < txt.txt | cat > michel.txt

char *read_from_fd(int fd) //pour attraper ce quil y a au bout du pipe
{
	unsigned int size;
	int i;
	
	char *buff = malloc(sizeof(char) * BUFF_SIZE);
	
	char *ret;
	
	ret = malloc(1);
	size = 0;
	
	while ( (i = (int) read(fd, buff, BUFF_SIZE) ) > 0)
	{
		ret = realloc(ret, size + i + 1);
		
		memcpy(&ret[size], buff, (size_t) i);
		
		size += i;
	}
	
	if (!size)
	{
		free(ret);
		return NULL;
	}
	
	ret[size] = '\0';
	return ret;
}

char *redir_cmd_pipe_first(char **argv[], char *in)
{
	int fd[2];
	char *buff;
	
	if(pipe(fd))
	{
		return NULL;
	}
	
	if (fork() == 0)
	{
		close(fd[0]);
		
		dup2(fd[1], STDOUT_FILENO);
		
		int fin = open(in,O_RDONLY);
		dup2(fin,STDIN_FILENO);
		
		close(fd[1]);
		
		execvp(argv[0][0], argv[0]);
		
		close(fin);
		
	}
	
	close(fd[1]);
	
	buff = read_from_fd(fd[0]);
	
	close(fd[0]);
	
	return buff;
}

/*
		if(out && in) // write only dans le out
		{
			int fin = open(in,O_RDONLY);
			int fout = open(out,O_WRONLY);
			int fildes[2];
			fildes[0] = fin;
			fildes[1] = fout;
			pipe(fildes);
			
			pid_t p = fork();
			int status;
			if (p == 0)
				execvp(argv[0],argv);
			else
				wait(&status);
		}
*/

void redir_cmd_pipe(char **argv[], char *in, char *out)
{
	if(in || out)
		printf("%s, redir_cmd_pipe_first",argv[0][0]);
	//appelle redir_cmd_pipe_first sur argv[0]
}


int main(int argc, char **argv)
{
	//ignore CTRL+Z and CTRL+C
	struct sigaction act;
	act.sa_handler = &handler;
	act.sa_flags = SA_RESTART;
	sigaction(SIGTSTP, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	
	if(argc > 1) // nom de fichier en argument
	{
		char *dir = malloc(sizeof(char) * 1024);
		
		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		int fd = open(argv[1],O_RDONLY);
		if (fd < 0)
			exit(EXIT_FAILURE);
		
		printf("%s$ ",getcwd(dir,1024));
		
		read(fd, s, 1024);
		
		parse_line(s,&tab);
		
		affiche_cmd(tab);
		
		simple_cmd(tab);
		
		free(dir);
		free(tab);
		free(s);
	}
	else while(1)
	{
		
		char *dir = malloc(sizeof(char) * 1024);
		
		char ***tab;
// 		char **tab = malloc( 100 * sizeof(char*));
		
		char *s = malloc(1024 * sizeof(char));
		
		char *in;
		char *out;
		
		char *ex;
		
		printf("%s$ ",getcwd(dir,1024));
		
		ex = fgets(s,1024,stdin);
		
		if(ex == 0) //Quit with CTRL+D
		{
			printf("\n");
			break;
		}
		
		parse_line_pipes(s,&tab, &in, &out);
		
		redir_cmd_pipe(tab, in, out);
		
		/*
		if(in || out)
			redir_cmd(*tab,in,out);
		else
			pipe_cmd(tab, in, out);
		*/
		
		printf("\n");
		
		free(dir);
		free(tab);
		free(s);
	}
	exit(EXIT_SUCCESS);
}