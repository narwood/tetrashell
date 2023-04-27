#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
	char *command = malloc(sizeof(char*) * 100);
	char *program = malloc(sizeof(char) * 50);
	char **my_args = malloc(sizeof(char*) * 5);
	char *token = malloc(sizeof(char*) * 50);
	int i = 1;

	for (int i = 0; i < 5; i++){
		my_args[i] = (char*) malloc(sizeof(char*));
	}

	printf("starting\n");
	while (strcmp(token, "exit") != 0){	
		printf("tetrashell> ");
		fgets(command, 100, stdin);
		printf("got command\n");
		command[strlen(command) - 1] = '\0';
		printf("added null terminator to command\n");
		program = strtok(command, " ");
		printf("set program to first arg\n");
		strcpy(token, program);	
		printf("strcopied program into token\n");
		strcpy(my_args[0], program);
		printf("set program to index 0\n");
		while (program = strtok(NULL, " ")){
			printf("in loop\n");
			strcpy(my_args[i], program);
			i++;
		}
		printf("took all args\n");
		my_args[i] = NULL;
		printf("nulled last arg\n");
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
			}
			   free(command);
			   printf("freed command\n");
			   program = my_args[0];
			printf("program name: %s\n", my_args[0]);
			   for (int i = 0; i < 5; i++){
			   free(my_args[i]);
			   }
			   printf("freed args list\n");
			 }

	return 0;}
