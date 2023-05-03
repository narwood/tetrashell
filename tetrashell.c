#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "tetris.h"
#include <dirent.h>
#include <math.h>
#include <limits.h>


static int SanityCheckState(TetrisGameState *s) {
	int row, col, tmp;
	char c;
	tmp = s->location_x;
	if ((tmp < 0) || (tmp >= BLOCKS_WIDE)) return 0;
	tmp = s->location_y;
	if ((tmp < PIECE_START_Y) || (tmp >= BLOCKS_TALL)) return 0;

	// Important check: make sure the current piece is a valid piece ID.
	tmp = sizeof(tetris_pieces) / sizeof(const char*);
	if ((s->current_piece < 0) || (s->current_piece >= tmp)) return 0;
	if ((s->next_piece < 0) || (s->next_piece >= tmp)) return 0;

	// Make sure the board contains no invalid characters.
	for (row = 0; row < BLOCKS_TALL; row++) {
		for (col = 0; col < BLOCKS_WIDE; col++) {
			c = s->board[row * BLOCKS_WIDE + col];
			if ((c < ' ') || (c > '~')) return 0;
		}
	}

	// There are clearly more sanity checks we can do, but these are all that
	// we'll test for now. Ideas for later:
	// - No completed lines
	// - Score is at least lines * 100.

	return 1;
}



int main(int argc, char* argv[]) {

	const int MAX_LINE = 4096;

	char filepath[MAX_LINE];
	char command[MAX_LINE];
	char *arg = malloc(sizeof(char) * 50);
	char **my_args = malloc(sizeof(char*) * 5);
	char *program = malloc(sizeof(char*) * 50);
	char abbr[5];
	int i = 1;


	char last_score[MAX_LINE];
	char last_lines[MAX_LINE];
	TetrisGameState last_game;
	TetrisGameState rn;
	int beenModified = 0;	
	FILE *fp;

	int is_rank = 0;
	int pip[2];


	for (int i = 0; i < 5; i++){
		my_args[i] = (char*) malloc(sizeof(char*));
	}

	printf("Welcome to \n");
	
    	char c;
	char s[2];
    	FILE* flower = fopen("flower.txt", "r");
    	if (flower == NULL) {
        	printf("File could not be opened\n");
        	//goto handle_error;
    	}	
    	while ((c = fgetc(flower)) != EOF) {
        	snprintf(s, sizeof(s), "%c", c);
		if (!strcmp(s, "(") || !strcmp(s, ")")) {
			printf("\033[33m%c", c);
		}
		else printf("\033[34m%c", c);
    	}
    	fclose(flower);
    
	printf("\033[0mEnter the path of the quicksave you would like to hack: ");
	fgets(filepath, 4096, stdin);
	filepath[strlen(filepath) - 1] = '\0';
	fp = fopen(filepath, "rb");
        if (fp == NULL) {
        	fprintf(stderr, "Could not open quicksave.\n");
                //goto handle_error;
        }
        printf("Quicksave set.\nEnter your command below:\n");
        fclose(fp);
	
	
	while (strcmp(program, "exit") != 1){

		fp = fopen(filepath, "rb");
		if (fp == NULL) {
                	fprintf(stderr, "Could not open quicksave.\n");
                	//goto handle_error;
        	}
		fread(&rn, sizeof(rn), 1, fp);
        	fclose(fp);
		
		int currentScore = rn.score;
		int currentLines = rn.lines;

		strncpy(abbr, filepath, 4);
		printf("%s@tetrashell[%s][%i/%i]> ", getlogin(), abbr, currentScore, currentLines);	
		fgets(command, MAX_LINE, stdin);
		command[strlen(command) - 1] = '\0';
		arg = strtok(command, " ");
		strcpy(program, arg);
		strcpy(my_args[0], arg);

		if (!strcmp(my_args[0], "exit")) {
			exit(1);		
		}

		if (!strcmp(my_args[0], "info")) {
			printf("Current savefile: %s\nScore: %i\nLines: %i\n", filepath, currentScore, currentLines);
		}

		if (!strcmp(my_args[0], "rank")) {
			if (pipe(pip) == -1){
				fprintf(stderr, "Pipe failed\n");
				//goto handle_error;
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

		if (!strcmp(arg, "switch")){
			char *oldfile = filepath;
			arg = strtok(NULL, " ");
			fp = fopen(arg, "rb");
			if (fp == NULL){
				fprintf(stderr, "File cannot be opened, check validity\n");
				return 1;}
			fread(&rn, sizeof(rn), 1, fp);
			fclose(fp);
			if (!SanityCheckState(&rn)){
				fprintf(stderr, "This file is insane. Request denied\n");
				return 1;}
			strcpy(filepath, arg);
			printf("Switched current quicksave from %s to %s\n", oldfile, filepath);
		}

		if (!strcmp(arg, "train")) {
				char guess[MAX_LINE];
				int guessnum;
				int difficulty = 2;
				int replay = 1;

				while(replay){
				int decimal, binary = 0, base = 1, remainder;
				decimal = (rand() % (int)(pow(10, difficulty) - pow(10, difficulty-1) + 1) + pow(10, difficulty-1));
		       		
				printf("What is this hex number in binary? 0x%x\n", decimal);
				fgets(guess, MAX_LINE, stdin);
				guess[strlen(guess) - 1] = '\0';
				guessnum = atoi(guess);
				while (decimal != 0) {
					remainder = decimal % 2;
					binary = binary + remainder * base;
					decimal = decimal / 2;
					base = base * 10;
				}
				printf("The answer is %d!\n", binary);
				if (guessnum == binary){
					printf("You got it! Next we'll try one that's a little harder. Would you like to continue? Type 'y' if yes and any key to exit.\n");
					++difficulty;
				} else { printf("Hmm, you weren't quite right. Next we'll try an easier one. Would you like to continue? Type 'y' if yes and any key to exit.\n");
					if (difficulty > 1){
						--difficulty;
					}
				}
				fgets(command, MAX_LINE, stdin);
				if ('y' != command[0]){
					replay = 0;}
}}
		else {


			pid_t pid = fork();
			if (pid) {

				int res;
				if (!strcmp(my_args[0], "rank")){
					close(pip[0]);
					if (write(pip[1], filepath, strlen(filepath)) == -1) {
						fprintf(stderr, "Write failed. errno: %i\n", errno);
						//goto handle_error;
					}
					close(pip[1]);}
				wait(&res);                 
				if (!strcmp(my_args[0], "recover")) {
					DIR *dr;
					struct dirent *en;

					//get fileCount, maxNameSize
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
						maxNameSize += 10;
						closedir(dr); //close all directory
					}
					else {
						fprintf(stderr, "Could not open directory\n");
						//goto handle_error;
					}

					//get number of digits in fileCount
					int power = 1;
					while (fileCount >= pow(10, power)) {
						power++;
					}
					int maxDigits = power;

					//get number of digits in maxScore
					power = 1;
					while (UINT_MAX >= pow(10, power)) {
						power++;
					}
					int maxScoreDigits = power;
					if (maxScoreDigits < 5) {
						maxScoreDigits = 5;
					}


					//print title
					for (int i = 0; i < (maxDigits + maxNameSize + 3 + 2*maxScoreDigits); i++) {
						printf("-");
					}
					printf("\n");
					printf("#");
					for (int i = 0; i < maxDigits; i++) {
						printf(" ");
					}
					printf("Filepath");
					for (int i = 0; i < (maxNameSize - 7); i++) {
						printf(" ");
					}
					printf("Score");
					for (int i = 0; i < (maxScoreDigits - 4); i++) {
						printf(" ");
					}
					printf("Lines\n");
					for (int i = 0; i < (maxDigits + maxNameSize + 3 + 2*maxScoreDigits); i++) {
						printf("-");
					}
					printf("\n");

					//loop again, add to array and print
					char **fileNames = malloc(sizeof(char) * MAX_LINE * fileCount);
					int index = 1;
					int color = 35;
					TetrisGameState state;
					dr = opendir("recovered");
					if (dr) {
						while ((en = readdir(dr)) != NULL) {
							if (color == 31){
								color = 33;
							} else if (color == 33){
								color = 32;
							} else if (color == 32){
								color = 34;
							} else if (color == 34){
								color = 35;
							} else if (color == 35){
								color = 31;
							}

							if(strcmp(en->d_name, ".") && strcmp(en->d_name, "..")) {

								fileNames[index-1] = en->d_name;
								char fileToOpen[MAX_LINE];
								snprintf(fileToOpen, MAX_LINE, "./recovered/%s", en->d_name);

								fp = fopen(fileToOpen, "rb");
								if(fread(&state, sizeof(state), 1, fp) == -1){
									fprintf(stderr, "Could not read file\n");}
								fclose(fp);

								//printing time :)
								printf("\033[%dm%d ", color, index);
								power = 1;
								while (index >= pow(10, power)) {
									power++;
								}
								for (int i = 0; i < maxDigits-power; i++) {
									printf(" ");
								}

								printf("\033[%dmrecovered/%s ", color, en->d_name);
								for (int i = 0; i < (maxNameSize - (10 + strlen(en->d_name))); i++) {
									printf(" ");
								}

								printf("\033[%dm%d ", color, state.score);
								power = 1;
								while ((state.score) >= pow(10, power)) {
									power++;
								}
								for (int i = 0; i < maxScoreDigits-power; i++) {
									printf(" ");
								}
								printf(" ");

								printf("\033[%dm%d\033[0m\n", color, state.lines);

								index += 1;

							}
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
							char setFile[MAX_LINE];
							snprintf(setFile, MAX_LINE, "./recovered/%s", fileNames[switchSave-1]);
							strcpy(filepath, setFile);
							printf("Done! Current quicksave is now %s\n", filepath);
						}
					}
					else {
						printf("Okay, quicksave not switched.\n");
					}
					free(fileNames);
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
						//goto handle_error;
					}
					my_args[i] = filepath;
					my_args[i+1] = NULL;
					if (strcmp(my_args[0], "recover") == 0){
						int path = open("/dev/null",  O_WRONLY);
						dup2(path, STDOUT_FILENO);
						dup2(path, STDERR_FILENO);
						close(path);}

					execv(my_args[0], my_args);
				}

				else if (!strcmp(my_args[0], "modify")) {
					if (i != 3) {
						fprintf(stderr, "Please enter a scoring metric and a number.\n");
						//goto handle_error;
					}

					fp = fopen(filepath, "rb");
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
					close(pip[1]);
					if (dup2(pip[0], 0) == -1) {
						fprintf(stderr, "invalid file\n");
						//goto handle_error;
					}
					close(pip[0]);
					if (i < 3)
					{	
						my_args[2] = "10";
						i++;
						if (i == 2){
							my_args[1] = "score";
							i++;
						}
					}
					my_args[i] = "uplink";
					my_args[i+1] = NULL;
					execv(my_args[0], my_args);
				}

			}

		}
	}
	//	free(filepath);
	//	free(command);
		free(arg);
		free(my_args);
		free(program);	

		return 0;
	}

