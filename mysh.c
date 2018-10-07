
/*
 * Robert Wais, Andrew Smith
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"

#define LINELEN 256

int main(int argc, char * argv[]) {

    /*
     * Parse Command line arguments to check if this is an interactive or batch
     * mode run.
     */


    //( 0 != (ret = parse_args_main(argc, argv)) )
    if (0 != (parse_args_main(argc, argv))) {
        fprintf(stderr, "Error: Invalid command line!\n");
        return -1;
    }

    /*
     * If in batch mode then process all batch files
     */
    if (TRUE == is_batch) {
        if (TRUE == is_debug) {
            printf("Batch Mode!\n");
        }

        //        ( 0 != (ret = batch_mode()) )
        if (0 != (batch_mode())) {
            fprintf(stderr, "Error: Batch mode returned a failure!\n");
        }
    }
    /*
     * Otherwise proceed in interactive mode
     */
    else if (FALSE == is_batch) {
        if (TRUE == is_debug) {
            printf("Interactive Mode!\n");
        }

        //(ret = interactive_mode())
        if (0 != (interactive_mode())) {
            fprintf(stderr, "Error: Interactive mode returned a failure!\n");
        }
    }
    /*
     * This should never happen, but otherwise unknown mode
     */
    else {
        fprintf(stderr, "Error: Unknown execution mode!\n");
        return -1;
    }


    /*
     * Display counts
     */
    printf("-------------------------------\n");
    printf("Total number of jobs               = %d\n", total_jobs);
    printf("Total number of jobs in history    = %d\n", total_history);
    printf("Total number of jobs in background = %d\n", total_jobs_bg);

    /*
     * Cleanup
     */
    //    if( NULL != batch_files) {
    //        free(batch_files);
    //        batch_files = NULL;
    //        num_batch_files = 0;
    //    }

    return 0;
}

int parse_args_main(int argc, char **argv) {

    //INTERACTIVE MODE
    if (argc < 2) {

        //BATCH MODE
    } else {
        is_batch = TRUE;

        //FILL argv into batch_files
        batch_files = &argv[1];
        num_batch_files = argc - 1;
    }

    /*
     * If no command line arguments were passed then this is an interactive
     * mode run.
     */

    /*
     * If command line arguments were supplied then this is batch mode.
     */

    return 0;
}

int batch_mode(void) {
    struct NodeList *list = listCreate();
    struct NodeList *jobs = listCreate();
    int result;
    int i;
    for (i = 0; i < num_batch_files; i++) {
        printf("Looking in file: %s\n", batch_files[i]);
        /*
         NOTE:  size_t is an unsigned int
         ssize_t is a signed int, signed so that it can return a -1
         */
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        /*
         * Read stdin, break out of loop if Ctrl-D
         */

        FILE *fptr;
        if ((fptr = fopen(batch_files[i], "r")) == NULL) {
            printf("Error! opening file");

            //BREAK FROM EVERYTHING
            return(-1);
        }

        //READ until EOF
        while ((read = getline(&line, &len, fptr)) != -1) {
            if ('\n' == line[strlen(line) - 1]) {
                line[strlen(line) - 1] = '\0';
            }
            //            printf("Line %s\n", line);

            char *token;
            token = strtok(line, " ");

            result = parseLine(token, line, list, jobs);
            if (result == 0) {
                return result;
            }
            //PARSE AND EXECUTE
        }

        //NOTE: FREE on our own because getline allocates the space
        free(line);
        printf("----------Reading New file----------\n");
    }
    /*
     * For each file...
     */

    /*
     * Open the batch file
     * If there was an error then print a message and move on to the next file.
     * Otherwise,
     *   - Read one line at a time.
     *   - strip off new line
     *   - parse and execute
     * Close the file
     */

    /*
     * Cleanup
     */


    return 0;
}

int interactive_mode(void) {
    struct NodeList *list = listCreate();
    struct NodeList *jobs = listCreate();
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    /*int background = 0; */
    int result;

    do{
        printf("mysh$ ");
        read = getline(&line,&len,stdin);

        //CHECK FOR EOF
        if(read == -1){
            return 0;
        }

        //Stripping newline
        if('\n' == line[strlen(line)-1]){
            line[strlen(line)-1] = '\0';
        }

        //TOKENIZE
        char *token;
        token = strtok(line," ");

        result = parseLine(token, line, list, jobs);
        if (result == 0) {
            return result;
        }
        //EXECUTE COMMAND
    }while(1==1);
    return 0;
}

int parseLine(char *token, char *line, struct NodeList *list, struct NodeList *jobs) {
    int background = 0;

    while (token != NULL) {
        if (strcmp(token, "exit") == 0) {
            //wait for background processes
            return 0;
        } else if (strcmp(token, "jobs") == 0) {
            //                listJobs(jobs);
            listJobs(jobs);
            token = strtok(NULL, " ");
        } else if (strcmp(token, "history") == 0) {
            //HISTORY
            listHistory(list);

            //READ NEXT
            token = strtok(NULL, " ");
        } else if (strcmp(token, "fg") == 0) {
            /* need to take in number for id */
            token = strtok(NULL, " ");
            builtin_fg((int) token, jobs);
            token = strtok(NULL, " ");
        } else if (strcmp(token, "wait") == 0) {
            return 0;
        } else {
            int i = 0;
            char **tempArgs = (char **) malloc(sizeof(char *) * 1);
            while (token != NULL) {
                //REALLOC'
                if (strcmp(token, ";") == 0) {
                    //SPLIT
                    token = strtok(NULL, " ");
                    break;
                } else if (strcmp(token, "&") == 0) {
                    token = strtok(NULL, " ");
                    background = 1;
                    break;
                }
                tempArgs = realloc(tempArgs, (sizeof(char *) * (i + 2)));
                //                    printf("Token: %s\n",token);

                /*If token == & || token == ; {
                 break; but part of line will be left
                 */


                tempArgs[i] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
                strcpy(tempArgs[i], token);
                token = strtok(NULL, " ");
                i++;
            }
            tempArgs[i] = NULL;

            struct job_t *job = jobCreate(line, i + 1, tempArgs, background, tempArgs[0]);
            listAdd(list, job);
            if (jobIsBackground(job) == 1) {
                setPosition(job, jobs->total + 1);
                listAdd(jobs, job);
            }
            launch_job(job, jobs);
            //RESET BACKGROUND
            background = 0;
        }
        //vvvvv LINE != NULL vvvvvv
    }

    return 1;
}

/*
 * You will want one or more helper functions for parsing the commands
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job, struct NodeList *jobs) {
    pid_t c_pid = 0;
    int status = 0;
    char *binary = jobBinary(loc_job);
    char **args = jobArgv(loc_job);
    /* fork a child process */
    c_pid = fork();

    /* Check for an error */
    if( c_pid < 0 ) {
        fprintf(stderr,
                "Error: Fork failed!\n");
        return -1; }
    /* Check if child */
    else if( c_pid == 0 ) {
        execvp(binary, args);
        /* exec does not return on success.
         * If we are here then error out */
        fprintf(stderr,
                "Error: Exec failed!\n");
        exit(-1);
    }
    /* Check if parent */
    else {
        //FOREGROUND
        if(jobIsBackground(loc_job) == 0){
            waitpid(c_pid, &status, 0);
        }else{
            //BACKGROUND
            setPID(loc_job,c_pid);
            waitpid(c_pid, &status, WNOHANG);
            setDone(loc_job);
        }

        //printf("Child finished!\n");
    }
    /*
     * Display the job
     */

    /*
     * Launch the job in either the foreground or background
     */

    /*
     * Some accounting
     */

    return 0;
}

int builtin_exit(void) {

    return 0;
}

int builtin_jobs(void) {

    return 0;
}

int builtin_history(void) {

    return 0;
}

int builtin_wait(void) {

    return 0;
}

int builtin_fg(int id, struct NodeList *jobs) {
    if (jobs->head == NULL) {
        /* error */
        return 0;
    } else {
        struct job_t *job = NULL;
        int status;

        if (id < 0) {
            /* no argument means select most recent job */
            job = jobs->tail->job;
        } else {
            int i = 0;
            struct Node *currentNode = jobs->head;

            while (i < id) {
                currentNode = currentNode->next;
                i++;
            }
            job = currentNode->job;
        }
        waitpid(job->pid, &status, 0);

        return 1;
    }
}



/*
 Job Functions
 */

struct job_t *jobCreate(char *full_command, int argc, char **argv, int is_background, char *binary){
    struct job_t *job;
    job = (struct job_t *)malloc(sizeof(struct job_t));
    job->full_command = (char *)malloc(sizeof(char)* (strlen(full_command)+1));
    strcpy(job->full_command, full_command);
    job->argc = argc;

    //QUESTION???
    //Allocate memory for each char pointer
    //Already Allocated MEM
    job->argv = argv;
    //    job->argv = malloc(sizeof(char*) * argc);
    //    int i;
    //    //Allocate memory for each *char
    //    for(i = 0; i<argc ; i++){
    //        job->argv[i] = (char *)malloc(sizeof(char)*(strlen(argv[i])+1));
    //    }
    job->is_background = is_background;
    job->binary = (char *)malloc(sizeof(char)*(strlen(binary)+1));
    strcpy(job->binary,binary);
    job->pid = 0;
    job->position = 0;
    job->running = 1;
    return job;
}

char *jobFullCommand(struct job_t *job) {
    return (job->full_command);
}

int jobArgc(struct job_t *job) {
    return job->argc;
}

char **jobArgv(struct job_t *job) {
    return job->argv;
}

int jobIsBackground(struct job_t *job) {
    return job->is_background;
}

char *jobBinary(struct job_t *job) {
    return job->binary;
}

void setPosition(struct job_t *job, int position){
    job->position = position;
}
void setPID(struct job_t *job, int PID){
    job->pid = PID;
}
void setRunning(struct job_t *job){
    job->running = 1;
}

void setDone(struct job_t *job){
    job->running = 0;
}

/*
 Node list functions
 */

struct NodeList *listCreate(){
    struct NodeList *list = malloc(sizeof(struct NodeList));
    list->size = 0;
    list->total = 0;
    list->head=NULL;
    list->tail=NULL;
    return list;
}

void listAdd(struct NodeList *list, struct job_t *job){
    if(list->head == NULL){
        struct Node *onlyElement = malloc(sizeof(struct Node));
        onlyElement->job = job;
        onlyElement->next = NULL;
        list->head = onlyElement;
        list->tail = onlyElement;
        //NON-EMPTY-LIST
    }else{
        //ADD NODE TO END OF THE LIST
        struct Node *newTail = malloc(sizeof(struct Node));
        struct Node *prevTail = list->tail;
        newTail->job = job;
        newTail->next = NULL;
        prevTail->next = newTail;
        list->tail = newTail;
    }
    list->size++;
    list->total++;
}

void listHistory(struct NodeList *list){
    struct Node *curr = list->head;
    if(list->head == NULL){
        printf("LIST IS NULL\n");
        return;
    }
    int count = 1;
    while(curr != NULL){
        printf("%d %s\n",count,jobBinary(curr->job));
        curr = curr->next;
        count++;
    }
}

void listJobs(struct NodeList *list){
    int status = 0;
    struct Node *curr = list->head;
    struct Node *temp = NULL;
    struct Node *previous = NULL;
    if(list->head == NULL){
        return;
    }
    while(curr != NULL){
        if(waitpid(curr->job->pid, &status, WNOHANG)!=0){

            printf("[%d]\tDone\t",curr->job->position);
            if(previous == NULL){
                list->head=curr->next;
            }else{
                previous->next = curr->next;
            }
            //Print out
            int i;
            for(i = 0;i <curr->job->argc-1; i++){
                printf(" %s ",curr->job->argv[i]);
            }
            //FREE THE NODE
            temp = curr;
            curr=curr->next;
            free(temp);
            //FREE NODE
            list->size = list->size - 1;
            printf("\n");
        }else{
            printf("[%d]\tRunning\t",curr->job->position);
            int i;
            for(i = 0;i <curr->job->argc-1; i++){
                printf(" %s ",curr->job->argv[i]);
            }
            printf("\n");
            previous = curr;
            curr = curr->next;
        }
    }

    if(list->size==0){
        list->total = 0;
    }
}

void listRemove(struct NodeList *list, int pid){
    struct Node *curr = list->head;
    struct Node *previous = NULL;
    int i;
    for(i = 0; i<list->size; i++){
        if(curr->job->pid == pid){
            if(previous == NULL){
                list->head=curr->next;
            }else{
                previous->next = curr->next;
            }
            list->size = list->size - 1;
            break;
        }
        previous = curr;
        curr = curr->next;
    }
}
/*
 NOTE:The first call to strtok must pass the C string to tokenize, and subsequent calls must specify NULL as the first argument, which tells the function to continue tokenizing the string you passed in first.
 */
