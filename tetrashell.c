#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
char *command = malloc(sizeof(char) * 100);
char *program = malloc(sizeof(char) * 50);
char **my_args = (char**) malloc(sizeof(char*) * 5);
int i = 1;

for (int i = 0; i < 5; i++){
	my_args[i] = (char*) malloc(sizeof(char*));
}

printf("starting\n");
while (strcmp(program, "exit") != 0){	
	printf("tetrashell> ");
	fgets(command, 100, stdin);
	program = strtok(command, " ");
	strcpy(my_args[0], program);
	while (program = strtok(NULL, " ")){
	strcpy(my_args[i], program);
	++i;
	}
	strcpy(my_args[i+1], "\0");
	//my_args[1] = strtok(command, " ");
	
//scanf("%s", program);
	if(strcmp(my_args[0], "exit") == 0){
		exit(1);		
	}
	printf("line 20\n");


	pid_t pid = fork();
	if (pid){
	printf("in parent\n");
	int res;
	wait(&res);
	} else {
		printf("in child\n");	
		if (strcmp(my_args[0], "recover") == 0){
                        printf("recovering\n");
		}
		else if (strcmp(my_args[0], "check") == 0){
			printf("checking\n");
		}	

		printf("about to execv\n");
		execv(my_args[0], my_args);
		printf("returning to parent.\n");
		free(program);
		free(command);
		return 0;}}

return 0;}
