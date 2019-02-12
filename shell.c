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
	
int parse_line(char *s, char * *argv[]) //char* argv[]  plutot
{
// decoupe la chaine s terminée par \0, mots séparés dun espace dans case dif du tableau, 
// case NULL à la fin du tableau

	unsigned int i;
	unsigned int s_int;
	unsigned int argv_int;
	
	char* str;
	
	s_int = 0;
	argv_int = 0;
	
	str = malloc(sizeof(char) + 1);
	argv[argv_int] = malloc(sizeof(char) + 1);
	
	while ( s[s_int] != '\0' ){
		i = 0;
		while ( str[i] = s[s_int] != ' ' && s[s_int] != '\0'){
			str = realloc(str, i + 2);
			++i;
			++s_int;
		}
		str[i] = '\0';
		
		argv[argv_int] = malloc(strlen(str[i]) + sizeof(char));
		strcpy(str, argv[argv_int]);
		
		++argv_int;
	}
	
	++argv_int;
	argv[argv_int] = NULL;
	
	return 0;
	
}




int main(int argc, char **argv){
	printf("\n------ Affiche_cmd\n");
	
	affiche_cmd(argv);
// 	char * tab[100];//static
	char **tab = malloc( 100 * sizeof(char*));
	
	char *e = "commande -v toto  tata";
	char *s = malloc(1024 * sizeof(char));
	
	strcpy (s,e);
// 	parse_line(s,tab);
	
	
// 	parse_line(s,&tab);

	// on met \0 a la fin des mots on modifie la chaine s et on met dans tab les pointeurs vers chaque debut de mot
	// eviter alloc dynamique dans parse line
	exit(EXIT_SUCCESS);
	
}





//on met \0 a la fin des mots on modifie la chaine s et on met dans tab les pointeurs vers chaque debut de mot 

// int parse_line(char *s, char * *argv[])
// {
// // decoupe la chaine s terminée par \0, mots séparés dun espace dans case dif du tableau, 
// // case NULL à la fin du tableau
// 
// 	unsigned int i;
// 	unsigned int s_int;
// 	unsigned int argv_int;
// 	
// 	char* str;
// 	
// 	s_int = 0;
// 	argv_int = 0;
// 	
// 	str = malloc(sizeof(char) + 1);
// 	argv[argv_int] = malloc(sizeof(char) + 1);
// 	
// 	while ( s[s_int] != '\0' ){
// 		
// 		i = 0;
// 		
// 		while ( str[i] = s[s_int] != ' ' && s[s_int] != '\0'){
// 			str = realloc(str, i + 2);
// 			++i;
// 			++s_int;
// 		}
// 		str[i] = '\0';
// 	
// 		str[i] = argv[argv_int] ; // need malloc
// 		++argv_int;
// 	}
// 	
// 	return 0;
// 	
// }



// char *strpbrk(const char *s, const char *accept);
// 
// DESCRIPTION
//        The strpbrk() function locates the first occurrence in the string s of any of the bytes in the string accept.
