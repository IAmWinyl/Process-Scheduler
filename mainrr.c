
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


const int MAX_BUFFER_LEN = 100;

enum commands{use, processname, processcount, end, runfor, quantum};

enum schedulertype{fcfs, sjf, rr};

// Represents processes in a
typedef struct Process {
    char * name;
    int arrival;
    int burst;
    int end;
    int originalburst;

} process;

// basic linked list used for the process queue
typedef struct Node{
    process * proc;
    struct node * next;
}node;

typedef struct Queue{
    node * head;
    node * tail;

}queue;

// Process functions
process * createProcess(char *name, int burst, int arrival){
    process *p = malloc(sizeof(process));
    p->name = name;
    p->burst = burst;
    p->arrival = arrival;
    p->end = -1;
    p->originalburst = burst;
    return p;
}

process * freeProcess(process *p){
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
node * createNode(process * p){
    node *n = malloc(sizeof(node));
    n->next = NULL;
    n->proc = p;
}

node * freeNode(node *n){
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
    node * oldhead = NULL;
    node *oldtail = NULL;
    node *temp = NULL;

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
    node *temp = q->head;
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
queue * appendQueue(queue *q, process *p){
    node *n = createNode(p);
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

    node * temp = NULL;
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
    node *n1 = NULL;
    node *n2 = NULL;

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
                    command = use;
                    break;
                case 'r':
                    command = runfor;
                    break;
                case 'q':
                    command = quantum;
                    break;
                case 'p':
                    if(str[i+7] == 'c'){
                        command = processcount;
                    }else{
                        command = processname;
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

            if(command == use){
                line = strstr(line, "use");

                switch(line[4]){
                    // this character should contain the first letter of the use value
                    case 'f':
                        // first come first serve value
                        temp = fcfs;
                        break;

                    case 's':
                        // shortest job first value
                        temp = sjf;
                        break;

                    case 'r':
                        // round robin value
                        temp = rr;
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
    i++;
    procnameptr[i] = '\0';

    //printf("name of the process is: %s\n", procnameptr);
    return procnameptr;
}

// gets the first process string it finds in procFile, makes a process object with it, then returns the object. returns NULL if no process found
// Assumptions - each process string uses correct syntax
process * getProcess(FILE * procFile){
    process *proc = NULL;

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

        if(commandtype == processname) {
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
process ** sortProcesses(process ** processes, int len)
{
    int i, j;
    process * temp = NULL;
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
process ** getAllProcesses(FILE *procFile){

    process **processes = NULL;

       int i = 0;

    int *temp = getCommandVal(procFile, processcount);
    int procCount = *temp;


    if(procCount != NULL){

        // make a process array
        processes = malloc(procCount*sizeof(process));

        // get all processes from the processes.in file and add them to the processes array
        for(i = 0; i < procCount; i++){

            processes[i] = getProcess(procFile);
        }
    }

    return processes;
}

void printprocess(process *p){
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

// Round Robin yummmmmmmmmmmm
// if outputFile is NULL, it prints the output to console, otherwise, it prints the output to the outputFile
int roundRobin(FILE * processFile, FILE * outputFile){
    int *tempv = NULL;
    int useval;
    int pcval;
    int rfval;
    int qval;
    int i = 0; // processarray index
    int currentprocessindex = -1;
    int nextprocessindex = -1;
    int currentime = 0;
    int cpuisidle = 1;
    int newprocselected = 0;

    queue * processqueue = NULL;
    process * procptr = NULL;
    node * tempnode = NULL;

    process **processes = NULL;



    // go to start of proc file
    rewind(processFile);

    // get use command value
    tempv = getCommandVal(processFile, use);
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
    tempv = getCommandVal(processFile, runfor);
    if(tempv != NULL){
        rfval = *tempv;
//        printf("run for command value is %d\n", rfval);
    }

    // if use command is rr, get quantum command value
    if (useval = rr) {
        // get quantum command value
        tempv = getCommandVal(processFile, quantum);
        if (tempv != NULL) {
            qval = *tempv;
//            printf("quantum command value is %d\n", qval);
        }
//        printf("\n");

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
            process *original[pcval];
            //original = malloc(sizeof(pcval*sizeof(process*)));


            for(int j = 0; j < pcval; j++){
                original[j] = processes[j];

            }


            // sort the process array
            sortProcesses(processes, pcval);


            // processes should now be sorted by ascending arrival
            if (processes != NULL) {
                currentprocessindex = 0;
                i = 0;
                int qctr = qval;

                process * currentproc = NULL;
                process * prevproc = NULL;

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


                for(int j = 0; j < pcval; j++){
                    // don't include new line character in the last line
                    if(j != pcval){
                        printf("     %s wait %d turnaround %d\n", original[j]->name, calcWait(original[j]->arrival,original[j]->end,original[j]->originalburst), calcTurnaround(original[j]->arrival,original[j]->end));
                        fprintf(outputFile,"     %s wait %d turnaround %d\n", original[j]->name, calcWait(original[j]->arrival,original[j]->end,original[j]->originalburst), calcTurnaround(original[j]->arrival,original[j]->end));
                    }else{
                        printf("     %s wait %d turnaround %d", original[j]->name, calcWait(original[j]->arrival,original[j]->end,original[j]->originalburst), calcTurnaround(original[j]->arrival,original[j]->end));
                        fprintf(outputFile,"     %s wait %d turnaround %d", original[j]->name, calcWait(original[j]->arrival,original[j]->end,original[j]->originalburst), calcTurnaround(original[j]->arrival,original[j]->end));

                    }

                }


            } else {
                printf("processes is NULL!\n");

            }

        }
    }

    return 0;
}



// everything above needs to be included, everything below does not and is just an example of how to use the function
// program POE
int main() {

    // Open the process.in file as read only
    FILE *processFile = NULL;
    processFile = fopen("processes.in", "r");
    
    // Create/open the processes.out file
    FILE * outputFile = NULL;
    outputFile = fopen ("processes.out","w");

    // run the rr scheduler
    roundRobin(processFile, outputFile);

    // close the processes.in file stream
    fclose(processFile);

    // close the output file
    fclose(outputFile);

    // program exited cleanly, return 0;
    return 0;
}

 
