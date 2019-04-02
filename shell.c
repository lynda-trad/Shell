#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define BUFF_SIZE 100

static void handler()
{
	//ignores the signals CTRL+Z and CTRL+C
}

void affiche_cmd(char *argv[])
{ //prints a command
	unsigned int i;
	i = 0;
	
	while(argv[i])
	{
		printf("%s",argv[i]);
		++i;
	}
	printf("\n");
}

void affiche_cmd_pipe(char ***str)
{ //prints a command that contains a pipe
	unsigned int i;
	
	i = 0;
	while (str[i])
	{
		affiche_cmd(str[i]);
		++i;
	}
}

void set_envi(char **argv[])
{ // sets a value to an environnemental variable
	if(argv[0][0] && argv[0][1])
	{
		if(setenv(argv[0][0],argv[0][1], 1) < 0)
		{
			fprintf(stderr,"fail to setenv\n");
			exit(EXIT_FAILURE);
		}
	}
}

void check_envi(char **argv[])
{ //remplaces occurences of words by the value of the environnemental variable
	unsigned int i;
	while(argv[0][i])
	{	
		if(getenv(argv[0][i]))
		{
			//change argv[0][i] en la valeur
			argv[0][i] = realloc(argv[0][i], sizeof(char) * strlen(getenv(argv[0][i])) + 1);
			strcpy(argv[0][i], getenv(argv[0][i])); 
		}
		++i;
	}
}

int parse_line(char *s, char **argv[])
{ // parses the line s
	unsigned int i;
	unsigned int len;
	unsigned int wordl;
	unsigned int envi;
	
	char **tmp;
	char *debw;
	
	i = 0;
	len = 0;
	tmp = malloc(sizeof(char*) * 1);
	
	envi = 0;
	
	while(s[i] && s[i] != '\n')
	{
		while (s[i] == ' ')
		{
			++i;
		}
		
		debw = &s[i];
		wordl = 0;
		
		while(s[i] && s[i] != ' ' && s[i] != '\n' && s[i] != '=')
		{
			++wordl;
			++i;
		}
		
		if(s[i] == '=')
		{
			++envi;
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
	
	
	//environnemental values handling
	if(envi > 0)
		set_envi(argv);
	else if(argv[0][0])
		check_envi(argv);
	
	return len;
}

void simple_cmd(char *argv[])
{ // executes a simple command
	if(!*argv)
		return;
	
	if(!strcmp(argv[0],"exit"))
	{
		exit(EXIT_SUCCESS);
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
{ // parses the line to get the in and out for redirections
	char *debw;
	unsigned int wordl;
	
	wordl = 0;
	
	if(s[i] == '<')
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
	else if(s[i] == '>')
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
{ //parses a line to get a command that contains redirections
	unsigned int i;
	unsigned int len;
	unsigned int wordl;
	unsigned int envi;
	
	char **tmp;
	char *debw;
	
	i = 0;
	len = 0;
	tmp = malloc(sizeof(char*) * 1);
	
	in[0] = NULL;
	out[0] = NULL;
	
	while(s[i] && s[i] != '\n' && s[i] != '|')
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
		
		while(s[i] && s[i] != ' ' && s[i] != '\n' && s[i] != '|' && s[i] != '=')
		{
			++wordl;
			++i;
		}
		
		if(s[i] == '=')
		{
			++envi;
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
	
	//environnemental values handling
	if(envi > 0)
		set_envi(argv);
	else if(argv[0][0])
		check_envi(argv);
	
	return i;
}


int redir_cmd(char *argv[], char *in, char *out)
{ //executes a command that may contain redirections
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
	else	if(out)
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
	else if(in)
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
	else 
		simple_cmd(argv);
	
	return 0;
}


unsigned int skip_space(const char *s, unsigned int i)
{ //skips spaces in a line
	while ( s[i] == ' ' || s[i] == '\t')
	{
		++i;
	}
	return i;
}

int parse_line_pipes(char *s, char ***argv[], char **in, char **out)
{ // parses a line that contains pipes and, sometimes, redirections
	unsigned int i;
	unsigned int len;
	char *unused; 		//in only possible for the first command
	
	len = 1;
	argv[0] = malloc(sizeof(char **) * 2);
	i = skip_space(s, 0);
	i += (unsigned int) parse_line_redir(&s[i], argv[0], in, out);
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

char *read_from_fd(int fd)
{ //reads from a file descriptor
	unsigned int size;
	int i;
	char buff[BUFF_SIZE];
	
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


void redir_cmd_pipe(char **argv[], char *in, char *out)
{ //executes a command that contains pipes and, sometimes, redirections
	unsigned int i;
	
	int fd_in[2];
	int fd_out[2];
	
	int fd_bkp[2];
	
	char *buff;
	
	int fin;
	int fout;
	
	i = 0;
	
	fd_bkp[0] = dup(STDIN_FILENO);				//backup of standard input
	fd_bkp[1] = dup(STDOUT_FILENO);				//backup of standard output
	
	buff = NULL;
	
	if(in)
	{
		fin = open(in,O_RDONLY);
		
		if(fin > 0)
		{
			buff = read_from_fd(fin);
			close(fin);
		}
		else
		{
			fprintf(stderr,"open failed\n");
			exit(EXIT_FAILURE);
		}
	}
	
	while (argv[i])
	{
		if(pipe(fd_in) || pipe(fd_out))
			return;
		
		if (fork() == 0)
		{
			close(fd_out[0]);					//child doesnt read its output
			close(fd_in[1]);					//child doesnt write its input
			
			dup2(fd_in[0], STDIN_FILENO);			//child binds its input reading to stdin
			
			close(fd_in[0]);
			
			dup2(fd_out[1], STDOUT_FILENO);		//child binds its input reading to stdin
			
			close(fd_out[1]);
			
			execvp(argv[i][0], argv[i]);
		}
		
		close(fd_out[1]);						//dad doesnt write child output
		close(fd_in[0]);						//dad doesnt read child input
		
		if (buff)
		{
			if(!argv[i+1] && out)
			{
				fout = open(out,O_WRONLY|O_TRUNC|O_CREAT);
				
				if(fout > 0)
				{
					write(fout,buff, strlen(buff));
					close(fout);
				}
				else
				{
					fprintf(stderr,"open failed\n");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				write(fd_in[1], buff, strlen(buff));	//dad writes to child's input
				free(buff);
			}
		}
		
		close(fd_in[1]);
		
		wait(NULL);
		
		buff = read_from_fd(fd_out[0]);			//dad reads what child printed
		
		close(fd_out[0]);
		
		dup2(fd_bkp[0], STDIN_FILENO);			//restoring standard input
		dup2(fd_bkp[1], STDOUT_FILENO);			//restoring standard output
		
		++i;
	}
	
	if(buff)
		printf("%s",buff);
	
}


void which_cmd(char *s, char **argv[], char *in, char *out)
{ //chooses which function to call to execute a command
	unsigned int i;
	unsigned int cmpt;
	
	i = 0;
	cmpt = 0;
	
	while(s[i])
	{
		if(s[i] == '|')
			++cmpt;
		++i;
	}
	
	if(cmpt != 0)
		redir_cmd_pipe(argv, in, out);
	else
		redir_cmd(argv[0], in, out);
}


int main(int argc, char **argv)
{
	//ignores CTRL+Z and CTRL+C
	struct sigaction act;
	act.sa_handler = &handler;
	act.sa_flags = SA_RESTART;
	sigaction(SIGTSTP, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	
	if(argc > 1) 
	{ //executes a command from a file put in the arguments
		char ***tab;
		
		char *s = malloc(1024 * sizeof(char));
		
		char *in;
		char *out;
		
		int fd;
		fd = open(argv[1],O_RDONLY);
		
		if (fd < 0)
		{	
			fprintf(stderr,"can not open file");
			exit(EXIT_FAILURE);
		}
		
		read(fd, s, 1024);
		
		parse_line_pipes(s, &tab, &in, &out);
		which_cmd(s, tab, in, out);
		
		printf("\n");
		
		free(tab);
		free(s);
		free(in);
		free(out);
	}
	else while(1)
	{ //asks a command to the user
		char *dir;
		char *s;
		char *ex;
		
		char ***tab;
		
		char *in;
		char *out;
		
		tab = malloc( 100 * sizeof(char**));
		
		dir = malloc(sizeof(char) * 1024);
		s = malloc(1024 * sizeof(char));
		
		printf("%s$ ",getcwd(dir,1024));
		ex = fgets(s,1024,stdin);
		
		if(ex == 0) 
		{ //quit with CTRL+D
			printf("\n");
			break;
		}
		
		parse_line_pipes(s, &tab, &in, &out);
		which_cmd(s, tab, in, out);
		
		printf("\n");
		
		free(dir);
		free(tab);
		free(s);
		free(in);
		free(out);
	}
	exit(EXIT_SUCCESS);
}