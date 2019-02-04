#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv){
	
	printf("------ Terminal\n");
	for(int i=0;i<argc;i++)
		printf("%s ",argv[i]);
	printf("\n %d arguments", argc-1);	
	
	printf("\n------ Affiche_cmd\n");
	
	char *s = malloc(1024 * sizeof(char));
	char *argv[] = malloc(100 * sizeof(char));
	
	affiche_cmd(argv);
	
	exit(EXIT_SUCCESS);
	
}




//   Si on doit faire un autre tableau 
// 	char **tab = malloc(sizeof(char));
// 	
// 	for(int i = 1; i < argc; i++){
// 		
// 		tab = realloc(tab, sizeof(argv[i]));
// 		tab[i-1] = argv[i];
// 		
// 	}
// 	
// // 	tab = realloc(tab, sizeof(char));
// 	
// 	tab[argc] = NULL; 
// 	
// //	segfault à partir de argc >= 3 why ? (realloc)
