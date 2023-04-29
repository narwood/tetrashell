
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "tetris.h"
#include <dirent.h>

int main(int argc, char* argv[]) {

	const int MAX_LINE = 4096;

	char *filepath = malloc(sizeof(char) * MAX_LINE);
	char *command = malloc(sizeof(char) * MAX_LINE);
	char *arg = malloc(sizeof(char) * 50);
	char **my_args = malloc(sizeof(char*) * 5);
	char *program = malloc(sizeof(char*) * 50);
	char abbr[5];
	int i = 1;


	char last_score[MAX_LINE];
	char last_lines[MAX_LINE];
	TetrisGameState last_game;
	int beenModified = 0;
	FILE *fp;

	int is_rank = 0;
	int pip[2];


	for (int i = 0; i < 5; i++){
		my_args[i] = (char*) malloc(sizeof(char*));
	}

	printf("Welcome to tetrashell!\nEnter the path of the quicksave you would like to hack: ");
	fgets(filepath, 4096, stdin);
	filepath[strlen(filepath) - 1] = '\0';
	printf("Quicksave set.\nEnter your command below:\n");

	while (strcmp(program, "exit") != 1){
		strncpy(abbr, filepath, 4);
		printf("%s@tetrashell[%s]> ", getlogin(), abbr);	
		fgets(command, MAX_LINE, stdin);
		command[strlen(command) - 1] = '\0';
		arg = strtok(command, " ");
		strcpy(program, arg);
		strcpy(my_args[0], arg);

		if (strcmp(my_args[0], "exit") == 0) {
			exit(1);		
		}

		if (!strcmp(my_args[0], "rank")) {
			if (pipe(pip) == -1){
				fprintf(stderr, "Pipe failed\n");
				return 1;
			}			                        
		}
		if (!strcmp(my_args[0], "undo")) {
			if (beenModified == 1) {
				//modify lines	
				pid_t pid = fork();
				if (pid) {
					int res;
					wait(&res);
				} else {
					my_args[0] = "modify";
					my_args[1] = "score";
					my_args[2] = last_score;
					execv("modify", my_args);
				}
				//modify score
				pid = fork();
				if (pid) {
					int res;
					wait(&res);
				} else {
					my_args[0] = "modify";
					my_args[1] = "lines";
					my_args[2] = last_lines;
					execv("modify", my_args);
				}

				printf("Previous quicksave restored.\n");
			}
		}
/*		if (!strcmp(arg, "info")){
			printf("Current savefile: %s\nScore: %i\nLines: %i\n", filepath, filepath->score, filepath->lines)
		}*/
		if (!strcmp(arg, "help")){
			arg = strtok(NULL, " ");
			if (!strcmp(arg, "check")){
				printf("Check determines if the inputted quicksave is valid or not.\n");
			} else if (!strcmp(arg, "modify")){
                                printf("Modify allows you to alter the score and lines values of your quicksave.\n");
                        } else if (!strcmp(arg, "rank")){
                                printf("Rank displays your quicksave on the internal leaderboard.\n");
                        } else if (!strcmp(arg, "exit")){
                                printf("Exit will end the program.\n");
                        } else if (!strcmp(arg, "undo")){
                                printf("Undo will revert your last modify change.\n");
                        } else if (!strcmp(arg, "recover")){
                                printf("Recover quicksaves from a disk image.\n");
                        }
		}
		else {


		pid_t pid = fork();
		if (pid) {
			
			int res;
			if (!strcmp(my_args[0], "rank")){
                                close(pip[0]);
                                if (write(pip[1], filepath, strlen(filepath)) == -1) {
                                        fprintf(stderr, "Write failed. errno: %i\n", errno);
                                        return 1;
                                }
			close(pip[1]);
			waitpid(pid, &res, 0);                        
}
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
					my_args[i] = filepath;
					my_args[i+1] = NULL;
                                        execv(my_args[0], my_args);
				}

				else if (!strcmp(my_args[0], "modify")) {
					if (i != 3) {
						fprintf(stderr, "Please enter a scoring metric and a number.\n");
						return 1;
					}

					fp = fopen("tetris_quicksave.bin", "rb");
					fread(&last_game, sizeof(last_game), 1, fp);
					fclose(fp);
					snprintf(last_score, MAX_LINE, "%d", last_game.score);
					snprintf(last_lines, MAX_LINE, "%d", last_game.lines);
					beenModified = 1;
					my_args[i] = filepath;
					my_args[i+1] = NULL;
                                        execv(my_args[0], my_args);
				}

				else if (!strcmp(my_args[0], "rank")) {
					if (i != 3) {
						fprintf(stderr, "Please enter a ranking metric and a number.\n");
						return 1;
					}
					close(pip[1]);
					if (dup2(pip[0], 0) == -1) {
						fprintf(stderr, "invalid file\n");
						return 1;
					}
					close(pip[0]);
					my_args[i] = "uplink";
					my_args[i+1] = NULL;
                                	execv(my_args[0], my_args);
				}

			}
			
			//Anything you want to execute after execv goes here
			if (!strcmp(my_args[0], "recover")) {
				
				DIR *dr;
        			struct dirent *en;

   				//get fileCount
				int fileCount = 1;
				int maxNameSize = 0;
				dr = opendir("recovered"); //open dir
   				if (dr) {
      					while ((en = readdir(dr)) != NULL) {
      						fileCount += 1;
						if (strlen(en->d_name) > maxNameSize) {
							maxNameSize = strlen(en->d_name);
						}
					}
      					closedir(dr); //close all directory
   				}
				else {
                                        fprintf(stderr, "Could not open directory");
                                }

				//get number of digits in fileCount, max chars in fileNames
				
				//loop again, add to array and print
				char **fileNames = malloc(sizeof(char*) * fileCount);
			      	int index = 0;	
				dr = opendir("recovered");
				if (dr) {
					while ((en = readdir(dr)) != NULL) {
						fileNames[index] = en->d_name;
						
						//printing time :)
						printf("%d %s\n", fileCount, en->d_name); //print all directory name
						index += 1;	
					}
					closedir(dr);
				}


				printf("If you would like to switch to one of these quicksaves, type (y): ");
				fgets(command, MAX_LINE, stdin);
                		command[strlen(command) - 1] = '\0';
				if (!strcmp(command, "y")) {
					printf("Which quicksave? (Enter a number): ");
					fgets(command, MAX_LINE, stdin);
					command[strlen(command)-1] = '\0';
					int switchSave = atoi(command);
					if (switchSave < 1 || switchSave > fileCount) {
						fprintf(stderr, "Number not in range of recovered quicksaves.\n");
					}
					else {
						filepath = fileNames[switchSave-1];
						printf("Done! Current quicksave is now %s\n", filepath);
					}
				}
   				else {
					printf("Okay, quicksave not switched.\n");
				}
			}
		}
}

	//still need to free all our shit

	return 0;
}
