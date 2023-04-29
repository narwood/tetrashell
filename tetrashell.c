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
	int is_rank = 0;

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

		if (strcmp(my_args[0], "exit") == 0) {
			exit(1);		
		}
	
		if (!strcmp(my_args[0], "rank")) {
			
			
                        if (i != 3) {
                        	fprintf(stderr, "Please enter a ranking metric and a number.\n");
                        	return 1;
                        }
                        
			strcpy(my_args[i], "uplink");
                        my_args[i+1] = NULL;
                        //pipe filepath to rank stdin??
                        //run rank with execv, not sure if this comes before piping or after
                }

		else {

		pid_t pid = fork();
		if (pid) {
			int res;
			wait(&res);
		} else {

			while (arg = strtok(NULL, " ")) {
				strcpy(my_args[i], arg);
				i++;
			}

			//check that i is correct # - bunch of if loops
			if ((strcmp(my_args[0], "recover") == 0) || (strcmp(my_args[0], "check") == 0)) {
				if (i != 1) {
					fprintf(stderr, "Check and recover do not take arguments.\n");
					return 1;
				}
			}

			else if (!strcmp(my_args[0], "modify")) {
				if (i != 3) {
                                        fprintf(stderr, "Please enter a scoring metric and a number.\n");
                                        return 1;
                                }
			}
			
			my_args[i] = filepath;
			my_args[i+1] = NULL;
			execv(my_args[0], my_args);
			

		}}

		//still need to free all our shit
		
	}
	return 0;
}
