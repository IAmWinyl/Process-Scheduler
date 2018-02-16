// COP 4600 - Programming Assignment 1
// Student: Jonathan Killeen
// Instructor: Matthew B. Gerber Ph.D.

//This version of Round-Robin should not run the scheduler immediately upon the arrival of a new process, unless the CPU is currently idle.
//Your program should ignore everything on a line after a # mark and ignore additional spaces in input.

// Assignment: Implement the First-Come First-Served, preemptive Shortest Job First, and Round-Robin algorithms as for single processors.

// Assumptions
// no ambiguous case
// no identical arrival times
// processes.in contains at least 1 process
// processes.in contains 1 runfor value
// processes.in contains 1 use value
// if processes.in use value is rr then processes.in use value is rr
// values are assigned to
// processes.in ends with a "end"
// characters following a # characters in each line in process.in should be ignored
// process.in may or may not contain # characters
// only the following command words can be used in process.in: "end", "processcount", "runfor", "use", "quantum", "process name"
// use command can only have 1 of the following values: fcfs, sjf, or rr
// commands within process.in can be in any order
// there are no errors in the entry of the commands in process.in
// processes can have the same name
// duplicate processes can exist in a queue at the same time but have different remaining burst times
// all commands are immediately followed by a space and then their value

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


const int MAX_BUFFER_LEN = 100;

enum commands{euse, eprocessname, processcount, end, erunfor, equantum};

enum schedulertype{efcfs, esjf, err};

// Represents processes in a
typedef struct Process {
    char * name;
    int arrival;
    int burst;
    int end;
    int originalburst;

} rprocess;

// basic linked list used for the process queue
typedef struct node{
    rprocess * proc;
    struct node * next;
}node;

typedef struct Queue{
    struct node * head;
    struct node * tail;

}queue;


// Process functions
rprocess * createProcess(char *name, int burst, int arrival){
    rprocess *p = malloc(sizeof(rprocess));
    p->name = name;
    p->burst = burst;
    p->arrival = arrival;
    p->end = -1;
    p->originalburst = burst;
    return p;
}

rprocess * freeProcess(rprocess *p){
    if(p != NULL){
        if(p->name != NULL){
            // free the process name array
            //free(p->name);
        }

        // free the process
        // free(p);
    }

    // a returned NULL value signifies that the node was successfully freed
    return NULL;
}

// Node functions
struct node * createNode(rprocess * p){
    struct node *n = malloc(sizeof(struct node));
    n->next = NULL;
    n->proc = p;
}

struct node * freeNode(struct node *n){
    if(n != NULL) {
        if (n->proc != NULL) {
            // free the process in the node
            //freeProcess(n->proc);
        }
        // free the node
        //free(n);
    }

    // a returned NULL value signifies that the node was successfully freed
    return NULL;
}

// Queue functions
queue * rotateQueue(queue *q){
    struct node * oldhead = NULL;
    struct node *oldtail = NULL;
    struct node *temp = NULL;

    if(q != NULL && q->head != NULL && q->tail != NULL){
        // queue is not null, head is not null, tail is not null
        // 1 or more nodes in queue

        if(q->head == q->tail){
            // only 1 node in queue, do nothing
        }else{
            // 2 or more nodes in queue

            if(q->head->next == q->tail){
                // 2 nodes in queue, swap them
                oldhead = q->head;
                oldtail = q->tail;

                oldhead->next = NULL;
                q->tail = oldhead;

                oldtail->next = oldhead;
                q->head = oldtail;


            }else{
                // more than 2 nodes in queue, swap head and tail
                oldhead = q->head;

                q->head = q->head->next;

                q->tail->next = oldhead;
                oldhead->next = NULL;
                q->tail = oldhead;

            }
        }


    }else{
        // queue is either null or invalid, rotation not possible.
    }

    // return the queue
    return q;
}

// prints the queue
void printQueue(queue *q){
    struct node *temp = q->head;
    while(temp != NULL){
        if(q->head == q->tail){
            printf("%s ", temp->proc->name);
            break;
        }else{
            printf("%s ", temp->proc->name);
            temp = temp->next;
        }

    }
    printf("\n");
}

// adds a node to the end of the queue
queue * appendQueue(queue *q, rprocess *p){
    struct node *n = createNode(p);
    if(q->head == NULL){
        // queue is currently empty
        q->head = n;
        q->tail = n;
    }else{
        // queue already has 1 or more elements, add a new tail and point the old tail to the new one
        if(q->head == q->tail){
            // queue has 1 element
            q->tail->next = n;
            q->tail = n;
        }else{
            // queue has more than 1 element
            q->tail->next = n;
            q->tail = n;
        }

        q->tail->next = NULL;

    }

    return q;
};

// removes the head from queue and assigns a new one
queue * removeHead(queue *q){

    struct node * temp = NULL;
    if(q == NULL || q->head == NULL || q->tail == NULL){
        // queue is either empty or non existant, cannot remove a head

    }else{
        // queue exists and has at least  node

        if(q->head == q->tail){

            // only 1 node in queue
            //freeNode(q->head);
            q->head = NULL;
            q->tail = NULL;

        }else {
            // 2 or more nodes in queue, free the current head and make the next node in queue the new head

            if(q->head->next == q->tail){
                // 2 nodes in queue
                temp = q->head->next;
                //freeNode(q->head);
                q->head = temp;
                q->tail = temp;
                q->tail->next = NULL;

            }else{

                // more than 2 nodes in queue
                temp = q->head->next;
                //freeNode(q->head);
                q->head = temp;

            }

        }
    }

    return q;
};

// frees a queue
queue * freeQueue(queue *q){
    // used to navigate the queue
    struct node *n1 = NULL;
    struct node *n2 = NULL;

    if(q != NULL && q->head != NULL && q->tail != NULL){
        // queue is not null, head is not null, tail is not null
        // 1 or more nodes in queue
        if(q->head == q->tail){
            // only 1 node in queue, free it
            freeNode(q->head);
            q->head = NULL;
            q->tail = NULL;

        }else{
            // 2 or more nodes in queue, free them

            n1 = q->head;
            n2 = n1->next;

            do{
                // free the previous node

                free(n1);

                // current node becomes the previous node
                n1 = n2;

                // next node becomes the current node
                n2 = n2->next;

            }while (n2 != NULL);

        }

        // once the queue has been emptied, free it
        free(q);
    }

    return q;
}

queue * createQueue(){
    queue * q = NULL;
    q = malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}



// returns true if the char is a letter, false otherwise
int isLetter(char c){
    int isletter = 0;
    if((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')){
        isletter = 1;
    }
    return isletter;
}


// finds the command in a string and returns its enumerated equivalent
int getCommandType(char * str){
    int command = -1;
    int i = 0;

    while(str[i] != '\0' && str[i] != '#'){
//        printf("%c\n", str[i]);
        if(isLetter(str[i])){

            // found the command
            switch(str[i]){

                case 'e':
                    command = end;
                    break;
                case 'u':
                    command = euse;
                    break;
                case 'r':
                    command = erunfor;
                    break;
                case 'q':
                    command = equantum;
                    break;
                case 'p':
                    if(str[i+7] == 'c'){
                        command = processcount;
                    }else{
                        command = eprocessname;
                    }
                default:
                    // invalid command, program should not get here
                    break;
            }
            // don't go to next char
            break;
        }

        // go to next char
        i++;
    }

    return command;
};


// checks if a char is numeric
int isNumeric(char c){
    int isnumeric = 0;

    // range of numbers in ascii table
    if(c >= '0' && c <= '9'){
        isnumeric = 1;

    }
    return isnumeric;
}

// takes in a null terminated string and returns the first number it finds in it, returns NULL if a number could not be found
int * extractNuminStr(char *str){
    int i = 0; // line index
    int j = 0; // buffer index
    int numberInStr = 0;
    int returnnum = 0;
    int * returnnumptr = NULL;

    char buffer[MAX_BUFFER_LEN]; // buffer

    while(str[i] != '\0' && str[i] != '#'){

        if(isNumeric(str[i])){
            // the first number char of the first number has been found
            numberInStr = 1;
            break;
        }

        i++;
    }


    if(numberInStr) {
        // if there is at least 1 number in the string, add it
        for (j = 0; (str[i] != '\0' && str[i] != '\n' && str[i] != '#'), j < MAX_BUFFER_LEN; i++, j++) {
            if (isNumeric(str[i])) {
                // if it is numeric, add it to the buffer
                buffer[j] = str[i];

            } else {
                // the last number char of the first number has been found and added to buffer, bail
                buffer[j] = '\0';

                break;

            }
        }

//        printf("buffer is %s\n", &buffer);
        // convert buffer to int
        returnnum = atoi(buffer);

//        printf("buffer num is %d\n", returnnum);
        returnnumptr = &returnnum;


    }else{
        // the program should not get here
        printf("no number found in string");

    }

    return returnnumptr;
}


// get command val of a specified command
int * getCommandVal(FILE *procFile, int command){
    int lcommand = -1;
    int temp = 0;
    int * commandvalptr = NULL;

    int i = 0; // line index
    int j = 0; // buffer index

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // set procFile pointer to the start of the file
    rewind(procFile);

    // find use command line in file
    while ((read = getline(&line, &len, procFile)) != -1) {
        lcommand = getCommandType(line);

        if(lcommand == end){
            break;
        }

        if(lcommand == command){

            if(command == euse){
                line = strstr(line, "use");

                switch(line[4]){
                    // this character should contain the first letter of the use value
                    case 'f':
                        // first come first serve value
                        temp = efcfs;
                        break;

                    case 's':
                        // shortest job first value
                        temp = esjf;
                        break;

                    case 'r':
                        // round robin value
                        temp = err;
                        break;

                    default:
                        printf("Invalid value submitted for use command");
                        // the program should not get here at any point
                        break;

                }

                commandvalptr = &temp;

            }else{
                // desired command found, get its value

                commandvalptr = extractNuminStr(line);

                break;
            }

        }
    }

    // set procFile pointer to the start of the file
    rewind(procFile);

    return commandvalptr;
}


// stub function, might expand later. shouldn't really need to
int isValidProcStr(char * procstr){
    int isvalidprocstr = 0;
    isvalidprocstr = 1;
    return isvalidprocstr;
}

// gets the burst value of a process string
int * getBurst(char * procstr){
    const char *burstcommand = "burst";
    int * burstptr = NULL;
    int burst = 0;
    char * burstindex = NULL; // gives the occurence of the birst substring

    // search the string for a substring
    burstindex = strstr(procstr, burstcommand);
    if(burstindex){
        //burstindex = burstindex - procstr;
        burstptr = extractNuminStr(burstindex);
    }

    return burstptr;
}

// gets the arrival value of a process string
int * getArrival(char * procstr){
    const char *arrivalcommand = "arrival";
    int * arrivalptr = NULL;
    int arrival = 0;
    char * arrivalindex = NULL; // gives the occurence of the arrival substring

    // search the string for a substring
    arrivalindex = strstr(procstr, arrivalcommand);
    if(arrivalindex){
        //arrivalindex = arrivalindex - procstr;
        arrivalptr = extractNuminStr(arrivalindex);
    }

    return arrivalptr;
}

// gets the process name of a process string
char * getProcName(char * procstr){

    char *procnameptr = NULL;
    int start = -1;
    int end = -1;
    int len = 0;
    int i = 12;
    int j = 0;

    char * namestr = NULL;

    // get the index of the process name value
    namestr = strstr(procstr, "process name");
//    printf("%s", namestr);
    // get the start char of the process name value
    while(namestr[i] == ' '){
        i++;
    }
    start = i;


    // get the end char of the process name value;
    while(namestr[i] != ' ' && namestr[i] != '#' && namestr[i] != '\0'){
        i++;
    }
    end = i;


    len = end-start;
//    printf("\nstart %d\n", start);
//    printf("\nend %d\n", end);
//    printf("\nlen %d\n", len);


    // copy sub string process name to a new string
    procnameptr = malloc((len+1)*sizeof(char));

    for(i = 0, j = start; i < len; i++, j++){
        procnameptr[i] = namestr[j];
    }

    // end string with a terminating value
//    i++;
    procnameptr[i] = '\0';

//    printf("name of the process is: %s\n", procnameptr);
    return procnameptr;
}

// gets the first process string it finds in procFile, makes a process object with it, then returns the object. returns NULL if no process found
// Assumptions - each process string uses correct syntax
rprocess * getProcess(FILE * procFile){
    rprocess *proc = NULL;

    int i = 0; // str index
    int j = 0; // buffer index;

    int currentarrival = -1;
    int currentburst = -1;
    char *currentname = NULL;

    int commandtype = -1;

    int *burstptr = NULL;


    // buffer for proc string
    char buffer[MAX_BUFFER_LEN];

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // a valid process string must have least 7 substr elements (at least 6 breaks) -> "process name P1 arrival 3 burst 5"
    // get first proccess string

    while ((read = getline(&line, &len, procFile)) != -1) {
        commandtype = getCommandType(line);


        if(commandtype == end){
            // effective end of file reached
            break;
        }

        if(commandtype == eprocessname) {
            // get start of command

            currentarrival = *getArrival(line);
            currentburst = *getBurst(line);
            currentname = getProcName(line);

            proc = createProcess(currentname, currentburst, currentarrival);

//             found a process string


            // find 1 process and break
            break;
        }

    }

    return proc;
}


// bubble sort for process arrays
rprocess ** sortProcesses(rprocess ** processes, int len)
{
    int i, j;
    rprocess * temp = NULL;
    int s = len;
    for (i = 0; i < s - 1; i++)
    {
        for (j = 0; j < (s - 1-i); j++)
        {
            if (processes[j]->arrival > processes[j + 1]->arrival)
            {
                // swap
                temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    return processes;

}

// creates a process array using a process.in file and return it
rprocess ** getAllProcesses(FILE *procFile){

    rprocess **processes = NULL;

    int i = 0;

    int *temp = getCommandVal(procFile, processcount);
    int procCount = *temp;


    if(temp != NULL){

        // make a process array
        processes = malloc(procCount*sizeof(rprocess));

        // get all processes from the processes.in file and add them to the processes array
        for(i = 0; i < procCount; i++){

            processes[i] = getProcess(procFile);
        }
    }

    return processes;
}

void printprocess(rprocess *p){
    if (p != NULL) {
        printf("name of the process is: %s\n", p->name);
        printf("burst of the process is: %d\n", p->burst);
        printf("arrival time of the process is: %d\n", p->arrival);
        printf("end of the process is: %d\n", p->end);
        printf("\n");
    } else {
        printf("NULL process\n");
    }
}

// calculates the turnaround time of a process
int calcTurnaround(int start, int end){
    int ta = end - start;

    return ta;
}

// calculates the wait time of a process
int calcWait(int start, int end, int burst){

    return (calcTurnaround(start, end) - burst);
}

int roundRobinHelper(FILE * processFile, FILE * outputFile){

    int useval;
    int pcval;
    int rfval;
    int qval;
    int *tempv = NULL;

    // go to start of proc file
    rewind(processFile);

    // get use command value
    tempv = getCommandVal(processFile, euse);
    if(tempv != NULL){
        useval = *tempv;
//        printf("use command value is %d\n", useval);
    }

    // get processcount command value
    tempv = getCommandVal(processFile, processcount);
    if(tempv != NULL){
        pcval = *tempv;
//        printf("processcount command value is %d\n", pcval);
    }

    // get run for command value
    tempv = getCommandVal(processFile, erunfor);
    if(tempv != NULL){
        rfval = *tempv;
//        printf("run for command value is %d\n", rfval);
    }

    tempv = getCommandVal(processFile, equantum);
    if (tempv != NULL) {
        qval = *tempv;
//            printf("quantum command value is %d\n", qval);
    }

    roundRobin(useval, pcval, rfval, qval, outputFile, processFile);
    return 0;
}

// Round Robin yummmmmmmmmmmm
// if outputFile is NULL, it prints the output to console, otherwise, it prints the output to the outputFile
int roundRobin(int useval, int pcval, int rfval, int qval, FILE * outputFile, FILE *processFile){
    queue * processqueue = NULL;
    rprocess * procptr = NULL;
    struct node * tempnode = NULL;

    rprocess **processes = NULL;


    int i = 0; // processarray index
    int currentprocessindex = -1;
    int nextprocessindex = -1;
    int currentime = 0;
    int cpuisidle = 1;
    int newprocselected = 0;



    // if use command is rr, get quantum command value
    if (useval = err) {
        // get quantum command value


        printf("     %d processes\n", pcval);
        fprintf(outputFile,"     %d processes\n", pcval);
        printf("     Using Round-Robin\n");
        fprintf(outputFile,"     Using Round-Robin\n");
        printf("     Quantum %d\n\n", qval);
        fprintf(outputFile,"     Quantum %d\n\n", qval);

        if (0 && pcval == 1) {
            procptr = getProcess(processFile);
            printprocess(procptr);

        } else {
            // more than 1 process, create a queue
            processqueue = createQueue();

            processes = getAllProcesses(processFile);

            // backup the original orientation of the processes array
            rprocess *original[pcval];
            //original = malloc(sizeof(pcval*sizeof(process*)));

            int m = 0;
            for(m = 0; m < pcval; m++){
                original[m] = processes[m];

            }


            // sort the process array
            sortProcesses(processes, pcval);


            // processes should now be sorted by ascending arrival
            if (processes != NULL) {
                currentprocessindex = 0;
                i = 0;
                int qctr = qval;

                rprocess * currentproc = NULL;
                rprocess * prevproc = NULL;

                // run the scheduler for rfval time units
                for (currentime = 0; currentime < rfval; currentime++) {
                    // add new arrivals to queue
                    // if a process arrives, create a node, add it to the queue
                    if (i < pcval && processes[i]->arrival == currentime) {
                        printf("     Time %d: %s arrived\n", currentime, processes[i]->name);
                        fprintf(outputFile,"     Time %d: %s arrived\n", currentime, processes[i]->name);
                        //printprocess(processes[i]);

                        appendQueue(processqueue, processes[i]);

                        //                        printQueue(processqueue);
                        i++;
                    }


                    // if there are no processes to run, idle
                    if(processqueue->head == NULL) {
                        // there are no processes to run, idle
                        printf("     Time %d: Idle\n", currentime);
                        fprintf(outputFile,"     Time %d: Idle\n", currentime);
                    }else{
                        // there are 1 or more processes to run, select one
                        // situations that could occur here
                        // a process has just joined after an idle period
                        // a process has just joined after a non idle period
                        // current queue has 1 or more processes already in it

                        // select the first process from the process queue
                        currentproc = processqueue->head->proc;

                        // check if process is finished
                        if(currentproc->burst <= 0){
                            // process finished
                            printf("     Time %d: %s finished\n", currentime, currentproc->name);
                            fprintf(outputFile,"     Time %d: %s finished\n", currentime, currentproc->name);
                            // record end time in process
                            currentproc->end = currentime;

                            // remove the process from the queue
                            removeHead(processqueue);

                            // reset the qctr
                            qctr = qval;

                            // select a new one if one can be selected
                            // if there are no processes to run, idle
                            if(processqueue->head == NULL) {
                                // there are no processes to run, idle
                                printf("     Time %d: Idle\n", currentime);
                                fprintf(outputFile,"     Time %d: Idle\n", currentime);
                                continue;
                            }else{
                                // there is a process to run, select it
                                currentproc = processqueue->head->proc;
                            }
                        }else{
                            // process is not finished, select it
                        }

                        // check if current processes quantum slice is depleted
                        if(qctr <= 0){
                            // qctr depleted, select the next process in queue
                            rotateQueue(processqueue);

                            // reset the qctr
                            qctr = qval;

                            // if there are no processes to run, idle
                            if(processqueue->head == NULL) {
                                // there are no processes to run, idle
                                printf("     Time %d: Idle\n", currentime);
                                fprintf(outputFile,"     Time %d: Idle\n", currentime);
                                continue;
                            }else{
                                // there is a process to run, select it
                                currentproc = processqueue->head->proc;
                            }
                        }else{
                            // qctr not depleted, keep current process

                        }

                        //if(currentproc != prevproc){
                        if(qctr == qval){
                            printf("     Time %d: %s selected (burst %d)\n", currentime, currentproc->name, currentproc->burst);
                            fprintf(outputFile, "     Time %d: %s selected (burst %d)\n", currentime, currentproc->name, currentproc->burst);
                        }


                        // run the process for 1 time unit
                        currentproc->burst--;

                        // reduce the processes time slice by 1 time unit
                        qctr--;

                        prevproc = currentproc;
                    }

                }
                printf("     Finished at time %d\n\n", rfval);
                fprintf(outputFile,"     Finished at time %d\n\n", rfval);

                int y = 0;
                for(y = 0; y < pcval; y++){
                    // don't include new line character in the last line
                    if(y != pcval){
                        printf("     %s wait %d turnaround %d\n", original[y]->name, calcWait(original[y]->arrival,original[y]->end,original[y]->originalburst), calcTurnaround(original[y]->arrival,original[y]->end));
                        fprintf(outputFile,"     %s wait %d turnaround %d\n", original[y]->name, calcWait(original[y]->arrival,original[y]->end,original[y]->originalburst), calcTurnaround(original[y]->arrival,original[y]->end));
                    }else{
                        printf("     %s wait %d turnaround %d", original[y]->name, calcWait(original[y]->arrival,original[y]->end,original[y]->originalburst), calcTurnaround(original[y]->arrival,original[y]->end));
                        fprintf(outputFile,"     %s wait %d turnaround %d", original[y]->name, calcWait(original[y]->arrival,original[y]->end,original[y]->originalburst), calcTurnaround(original[y]->arrival,original[y]->end));

                    }

                }


            } else {
                printf("processes is NULL!\n");

            }

        }
    }

    return 0;
}




// MERGER START

#define FALSE 0
#define TRUE 1

// Struct to store our process information
typedef struct process {
    char* name;
    int arrival;
    int burst;
    int wait;
    int turnaround;
} process;

// Gloabl Variables
int pcount = 0;
int runfor = 0;
int type = 0;
int quantum = 0;

// List of our struct objects
process processList[10000];

// File out
FILE* output;

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


// First-Come-First-Served
int fcfs() {

    int queuePos = -1;
    process *queue;
    int running = FALSE, runningPos = -1;

    queue = malloc(sizeof(process)*pcount);

    // Loop time
    for(int i = 0; i < runfor; i++) {
        // Loop for arrivals
        for(int j = 0; j < pcount; j++){
            if(processList[j].arrival == i) {
                queuePos++;
                queue[queuePos] = processList[j];
                printf("Time %d: %s arrived\n", i, processList[j].name);
                fprintf(output, "Time %d: %s arrived\n", i, processList[j].name);
            }
        }
        // If a process is running, check if done or more burst is left
        if(running == TRUE) {
            if(queue[runningPos].burst == 1) {
                printf("Time %d: %s finished\n", i, queue[runningPos].name);
                fprintf(output, "Time %d: %s finished\n", i, queue[runningPos].name);
                queue[runningPos].turnaround = i - queue[runningPos].arrival;
                running = FALSE;
            }
            else {
                queue[runningPos].burst--;
            }
        }
        // If no process is running
        if(running == FALSE) {
            if(runningPos == queuePos) {
                printf("Time %d: Idle\n", i);
                fprintf(output, "Time %d: Idle\n", i);
            }
                // Loop for new process
            else if(queuePos != -1) {
                runningPos++;
                printf("Time %d: %s selected (burst %d)\n", i, queue[runningPos].name, queue[runningPos].burst);
                fprintf(output, "Time %d: %s selected (burst %d)\n", i, queue[runningPos].name, queue[runningPos].burst);
                queue[runningPos].wait = i - queue[runningPos].arrival;
                running = TRUE;
            }
        }
    }
    printf("Finished at time %d\n\n", runfor);
    fprintf(output, "Finished at time %d\n\n", runfor);


    // Turnaround and Wait time
    for(int i = 0; i <= queuePos; i++) {
        printf("%s wait %d turnaround %d\n",queue[i].name, queue[i].wait, queue[i].turnaround);
        fprintf(output, "%s wait %d turnaround %d\n",queue[i].name, queue[i].wait, queue[i].turnaround);
    }

    free(queue);
    fclose(output);

    return 1;
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
    /*
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
*/
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
                fprintf(output, "%d processes\n", pcount);
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
                        printf("Using First-Come-First-Served\n"); // DEBUG
                        fprintf(output, "Using First-Come-First-Served\n");
                        break;
                    case 's': // Shortest Job First
                        type = 2;
                        printf("Using Shortest Job First (Preemptive)\n"); // DEBUG
                        break;
                    case 'r': // Round Robin
                        type = 3;
                        printf("Using Round-Robin\n"); // DEBUG
                        fprintf(output, "Using Round-Robin\n");
                        break;
                }
            }
                // Quantum
            else if(strncmp(substr(buf[i],0,6),"quantum",7) == 0) {
                quantum = charToInt(buf[i], 7);
                printf("Quantum %d\n", quantum); // DEBUG
                fprintf(output, "Quantum %d\n", quantum);
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
                fprintf(output,"\n");
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
    /*
    for(int i = 0; i < pcount; i++){
        printf("Process Name: %s | Arrival: %d | Burst: %d\n",processList[i].name, processList[i].arrival, processList[i].burst);
    }
    */

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

// MERGER END





// program POE


int main(int argc, char *argv[]) {

    // Open the process.in file as read only
    FILE *processFile = processFile = fopen("processes.in", "r");
    FILE *outputFile = NULL;

    int * temp = getCommandVal(processFile,euse);
    int useval = *temp;


    // close the processes.in file stream
    fclose(processFile);

    switch(useval){
        case err:

            // Open the process.in file as read only
            processFile = fopen("processes.in", "r");

            outputFile = fopen ("processes.out","w");

            roundRobinHelper(processFile, outputFile);

            // close the processes.in file stream
            fclose(processFile);

            // close the output file
            fclose(outputFile);
            break;
        case esjf:

            output = fopen("processes.out","w");

            parseFile(readFile());
            sjf();
            break;
        case efcfs:

            output = fopen("processes.out","w");

            parseFile(readFile());
            fcfs();

            break;
    }

    return 0;
}


