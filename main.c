#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// Struct to store our process information
typedef struct process {
	char* name;
    int arrival;
    int burst;
} process;

// Gloabl Variables
int pcount = 0;
int runfor = 0;
int type = 0;
int quantum = 0;

// List of our struct objects
process processList[10000];

// Process Schedule Timeline

/*
    (Each array cell is one unit of time. For each unit of time (i) a process (k) is used,
    we store the process number (k) in every (i) cell.)
*/

int *processTimeline;

char** readFile();
int parseFile(char** buf);
char* substr(char* str, int startIndex, int endIndex);
int charToInt(char* str, int startIndex);
int fcfs();
int sjf();
int rr();

int main(int argc, char *argv[]) {

	parseFile(readFile());
    
    /*
    switch(type)
    {
        case 1:
            return fcfs();
			break;
            
        case 2:
            return sjf();
			break;
            
        case 3:
            return rr();
			break;

        case default:
        	printf("ERR: Invalid schedule algorithm.\n");
        	exit(-1);
        	break;
    }
    */

	return 0;
}

// First-Come-First-Served
int fcfs() {
	int queuePos = 0;
	process *queue;

	queue = calloc(sizeof(process),pcount);

	// Loop time
    for(int i = 0; i < runfor; i++) {
    	for(int j = 0; j < pcount; j++){
	       if(processList[j].arrival == i) {
				queue[queuePos++] = processList[j];
				printf("Time %d: Process P", i);
			}
		}

    }
}

// Preemptive Shortest Job First
int sjf()
{
    for(int i = 0; i < pcount; i++)
    {
        
    }
}

// Round-Robin
int rr()
{
    for(int i = 0; i < pcount; i++)
    {
        
    }
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
	int processPos = 0;
	int length, endIndex, startIndex;

	for(int i = 0; i < 64; i++){
		if(buf[i] != NULL) {

			// Processcount
			if(strncmp(substr(buf[i],0,11),"processcount",12) == 0) {
				pcount = charToInt(buf[i], 12);
				printf("%d processes\n", pcount); // DEBUG
			}
			// Runfor
			else if(strncmp(substr(buf[i],0,5),"runfor",6) == 0) {
				runfor = charToInt(buf[i], 6);
                
                processTimeline = malloc(runfor * sizeof(int));
				// printf("Run For: %d\n", runfor); // DEBUG
			}
			// Use
			else if(strncmp(substr(buf[i],0,2),"use",3) == 0) {
				switch(buf[i][3]) {
					case 'f': // First-Come-First-Served
						type = 1;
                        printf("Using First-Come-First-Served %d\n", type); // %d is for DEBUG
						break;
					case 's': // Shortest Job First
						type = 2;
                        printf("Using Shortest Job First (Preemptive) %d\n", type); // %d is for DEBUG
						break;
					case 'r': // Round Robin
						type = 3;
                        printf("Using Round-Robin %d\n", type); // %d is for DEBUG
						break;
				}
			}
			// Quantum
			else if(strncmp(substr(buf[i],0,6),"quantum",7) == 0) {
				quantum = charToInt(buf[i], 7);
				printf("Quantum %d\n", quantum); // DEBUG
			}
            // Each Processes's Information
            else if(strncmp(substr(buf[i],0,10),"processname",11) == 0) {
            	// Store name substring
            	endIndex = (strstr(buf[i],"arrival")) - (buf[i]);
            	processList[processPos].name = calloc(sizeof(char), endIndex - 11);
            	strcpy(processList[processPos].name,substr(buf[i], 11,endIndex - 1));

            	// Store arrival and burst substring
            	startIndex = endIndex;
            	endIndex = (strstr(buf[i],"burst")) - (buf[i]);
                processList[processPos].arrival = charToInt(substr(buf[i], startIndex, endIndex - 1),7);
                processList[processPos].burst = charToInt(buf[i], endIndex + 5);
                processPos++;
            }
            // End Case
            else if(strncmp(substr(buf[i],0,2),"end",3) == 0)
            {
                printf("\n");
                break;
            }
		}
		else
        {
            printf("\n");
			break;
        }
	}

	// DEBUG
	for(int i = 0; i < pcount; i++){
		printf("Process Name: %s | Arrival: %d | Burst: %d\n",processList[i].name, processList[i].arrival, processList[i].burst);
	}

    return 1;
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
