#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

	const int MAX_LINE = 4096;

	char *filepath = malloc(sizeof(char) * MAX_LINE);
	char *command = malloc(sizeof(char) * MAX_LINE);
	char *arg = malloc(sizeof(char) * 50);
	char **my_args = malloc(sizeof(char*) * 5);
	char *program = malloc(sizeof(char*) * 50);
	int i = 1;

	for (int i = 0; i < 5; i++){
		my_args[i] = (char*) malloc(sizeof(char*));
	}

	printf("Welcome to tetrashell!\nEnter the path of the quicksave you would like to hack: ");
	fgets(filepath, 4096, stdin);
	filepath[strlen(filepath) - 1] = '\0';
	printf("Quicksave set.\nEnter your command below:\n");

	while (strcmp(program, "exit") != 0){	
		
		printf("tetrashell> ");
		
		fgets(command, MAX_LINE, stdin);
		command[strlen(command) - 1] = '\0';
		
		arg = strtok(command, " ");
		strcpy(program, arg);
		strcpy(my_args[0], arg);
	/*	while (arg = strtok(NULL, " ")){
			strcpy(my_args[i], arg);
			i++;
		}
		my_args[i] = NULL;
*/
		if(strcmp(my_args[0], "exit") == 0){
			exit(1);		
		}
		printf("line 20\n");

		if ((strcmp(my_args[0], "recover") == 0) || (strcmp(my_args[0], "check") == 0)){
			printf("saw recover or check\n");
			pid_t pid = fork();
			if (pid){
				printf("in recover parent\n");
				int res;
				wait(&res);
			} else {
				my_args[1] = filepath;
				my_args[2] = NULL;
				printf("in recover child\n");
				execv(my_args[0], my_args);
			 	printf("you're not supposed to be in here\n");}

			}
/*			free(command);
			printf("freed command\n");
			program = my_args[0];
			printf("program name: %s\n", my_args[0]);
			for (int i = 0; i < 5; i++){
				free(my_args[i]);
			}
			printf("freed args list\n");
*/		}

		return 0;}
