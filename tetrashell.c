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
	char *token = malloc(sizeof(char) * 50);
	int i = 1;

	for (int i = 0; i < 5; i++){
		my_args[i] = (char*) malloc(sizeof(char*));
	}

	printf("starting\n");
	while (strcmp(token, "exit") != 0){	
		printf("tetrashell> ");
		fgets(command, 100, stdin);
		command[strlen(command) - 1] = '\0';
		program = strtok(command, " ");
		strcpy(token, program);	
		strcpy(my_args[0], program);
		while (program = strtok(NULL, " ")){
			strcpy(my_args[i], program);
			i++;
		}
		my_args[i] = NULL;	
		//strcpy(my_args[i+1], "\0");
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
				printf("saw recover\n");
				//my_args[0] = strcat("./", my_args[0]);                        
				//printf("set first arg to ./recover\n");
			}
			else if (strcmp(my_args[0], "check") == 0){
				printf("checking\n");
			}	
			for (int j = 0; j < i; j++){
				printf("%s\n", my_args[j]);
			}
			printf("about to execv\n");
			execv(my_args[0], my_args);
			//printf("you're not supposed to be in here\n");
			/*
			   free(command);
			   printf("freed command\n");
			   printf("program name: %s\n", my_args[0]);
			   program = my_args[0];
			   for (int i = 0; i < 5; i++){
			   free(my_args[i]);
			   }
			   printf("freed args list\n");
			 */
			return 0;}}

	return 0;}
