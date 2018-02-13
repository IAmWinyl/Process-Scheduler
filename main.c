#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Global Variables
int pcount = 0;
int runfor = 0;
int type = 0;
int quantum = 0;

char** readFile();
int parseFile(char** buf);
char* substr(char* str, int startIndex, int endIndex);
int charToInt(char* str, int startIndex);

int main(int argc, char *argv[]) {

	parseFile(readFile());

	return 1;
}

char** readFile() {
	FILE *fp;
	char c;
	char **fbuffer;
	int line = 0, pos = 0;

	// Allocate file buffer
	fbuffer = (char**) calloc(64, sizeof(char*));
	for(int i = 0; i < 64; ++i){
		fbuffer[i] = (char*) calloc(128, sizeof(char));
	}

	// Open file
	fp = fopen("processes.in","r");

	// Error-Check
	if(fp == NULL) {
		free(fbuffer);
		printf("ERR: Could not open input file 'processes.in'.\n");
		exit(-1);
	}
	if(fbuffer == NULL) {
		free(fbuffer);
		printf("ERR: Could not allocate file buffer.\n");
		exit(-1);
	}

	// Read file to buffer
	while((c = fgetc(fp)) != EOF) {
		// Skip spaces
		if(c == ' ') {
			while(c == ' '){
				c = fgetc(fp);
			}
		}
		// Store until newline, comment, or EOF
		if(isalpha(c) || isdigit(c)) {
			fbuffer[line][pos++] = c;
		}
		else {
			while(c != '\n' && c != EOF) {
				c = fgetc(fp);
			}
			line++;
			pos = 0;
		}
	}

	// DEBUG
	for(int i = 0; i < 64; i++){
		if(fbuffer[i][0] != '\0'){
			for(int j = 0; j < 128; j++){
				if(fbuffer[i][j] != '\0')
					printf("%c", fbuffer[i][j]);
				else
					break;
			}
		printf("\n");
		}
		else
			break;
	}
	printf("\n\n");

	fclose(fp);
	return fbuffer;
}

int parseFile(char** buf) {
	for(int i = 0; i < 64; i++){
		if(buf[i] != NULL) {

			// Processcount
			if(strncmp(substr(buf[i],0,11),"processcount",12) == 0) {
				pcount = charToInt(buf[i], 12);
				printf("Process Count: %d\n", pcount); // DEBUG
			}
			// Runfor
			else if(strncmp(substr(buf[i],0,5),"runfor",6) == 0) {
				runfor = charToInt(buf[i], 6);
				printf("Run For: %d\n", runfor); // DEBUG
			}
			// Use
			else if(strncmp(substr(buf[i],0,2),"use",3) == 0) {
				switch(buf[i][3]) {
					case 'f': // First-Come-First-Served
						type = 1;
						break;
					case 's': // Shortest Job First
						type = 2;
						break;
					case 'r': // Round Robin
						type = 3;
						break;
				}
				printf("Use: %d\n", type); // DEBUG
			}
			// Quantum
			else if(strncmp(substr(buf[i],0,6),"quantum",7) == 0) {
				quantum = charToInt(buf[i], 7);
				printf("Quantum: %d\n", quantum); // DEBUG
			}
		}
		else
			break;
	}

}

char* substr(char* str, int startIndex, int endIndex) {
	char* newStr;
	int length = endIndex - startIndex;

	newStr = calloc(length, sizeof(char));

	// Copy over each character
	for(int i = 0; i <= length; i++) {
		newStr[i] = str[startIndex + i];
	}

	return newStr;
}

int charToInt(char* str, int startIndex) {
	int eos = startIndex, tmp = 0, j = startIndex, final = 0, radix;

	// Get End-Of-String
	while(str[eos] != '\0') {
			eos++;
	}

	// Char to Int
	while(str[j] != '\0') {
		tmp = str[j] - '0';
		for(int k = 1; k < eos-j; k++){
			tmp *= 10;
		}
		final += tmp;
		j++;
	}

	return final;
}