#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
char *command = malloc(sizeof(char) * 100);
char *program;
char *my_args[4];
int i = 1;

while (strcmp(program, "exit") != 0){	
	printf("tetrashell> ");
	fgets(command, 100, stdin);
	program = strtok(command, " ");
	my_args[0] = program;
/*	while (program = strtok(NULL, " ")){
	strcpy(my_args[i], program);
	++i;*/
//	}
	my_args[i+1] = NULL;
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
		else if (strcmp(my_args[0], "check ") == 0){
			printf("checking\n");
		}	

		printf("about to execv\n");
		execv(my_args[0], my_args);
		printf("returning to parent.\n");
		free(program);
		free(command);
		return 0;}}

return 0;}
