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
            printf("Line %s\n", line);
            
            char *token;
            token = strtok(line," ");
            while (token != NULL) {
                printf("Token: %s\n", token);
                /*
                 NOTE:The first call to strtok must pass the C string to tokenize, and
                 subsequent calls must specify NULL as the first argument, which tells
                 the function to continue tokenizing the string you passed in first.
                 */
                token = strtok(NULL, " ");
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
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int pass = 0;
    
    do{
        printf("mysh$ ");
        read = getline(&line,&len,stdin);
        
        //CHECK FOR EOF
        if(read == -1){
            return 0;
        }
        
        if('\n' == line[strlen(line)-1]){
            line[strlen(line)-1] = '\0';
        }
        
        //TOKENIZE
        char *token;
        token = strtok(line," ");
        
        while(token != NULL){
            if(strcmp(token,"exit")==0){
                //wait for background processes
                return 0;
            }else if(strcmp(token,"jobs")==0){
                printf("<Print out jobs>");
                return 0;
            }else if(strcmp(token,"history")==0){
                //HISTORY
                listHistory(list);
                
                //READ NEXT
                token = strtok (NULL, " ");
            }else if (strcmp(token,"fg")==0){
                return 0;
            }else if(strcmp(token,"wait")==0){
                return 0;
            }else{
                int i=0;
                char **tempArgs = (char**)malloc(sizeof(char*) * 1);
                while( token != NULL ){
                    //REALLOC
                    tempArgs=realloc(tempArgs, (sizeof(char*) * (i+2)));
                    printf("Token: %s\n",token);
                    
                    /*If token == & || token == ; {
                     break; but part of line will be left
                     */
                    if(strcmp(token,";")==0){
                        //SPLIT
                        token = strtok (NULL, " ");
                        break;
                    }
                    
                    tempArgs[i] = (char *)malloc(sizeof(char)*(strlen(token)+1));
                    tempArgs[i] = token;
                    token = strtok (NULL, " ");
                    i++;
                }
                struct job_t *job = jobCreate(line, 2,tempArgs,0,tempArgs[0]);
                listAdd(list,job);
                launch_job(job);
                
            }
            //vvvvv LINE != NULL vvvvvv
        }
        //EXECUTE COMMAND
    }while(1==1);
    return 0;
}

/*
 * You will want one or more helper functions for parsing the commands
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job) {
    pid_t c_pid = 0;
    int status = 0;
    char *binary = jobBinary(loc_job);
    char **args = jobArgv(loc_job);
    int i;
    for(i =0;i<jobArgc(loc_job);i++){
        printf("Arg: %s\n", args[i]);
    }
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
        waitpid(c_pid, &status, 0);
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

int builtin_fg(void) {
    
    return 0;
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

/*
 Node list functions
 */

struct NodeList *listCreate(){
    struct NodeList *list = malloc(sizeof(struct NodeList));
    list->size = 0;
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
        list->size++;
        //NON-EMPTY-LIST
    }else{
        struct Node *currHead = list->head;
        struct job_t *hope=currHead->job;
        printf("Current head: %s\n",jobBinary(hope));
        //ADD NODE TO END OF THE LIST
        struct Node *newTail = malloc(sizeof(struct Node));
        struct Node *prevTail = list->tail;
        newTail->job = job;
        newTail->next = NULL;
        prevTail->next = newTail;
        list->tail = newTail;
        list->size++;
    }
    printf("SIZE OF LIST: %d\n",list->size);
}

void listHistory(struct NodeList *list){
    struct Node *curr = list->head;
    if(list->head == NULL){
        return;
    }
    int count = 1;
    while(curr != NULL){
        printf("%d %s\n",count,jobBinary(curr->job));
        curr = curr->next;
        count++;
    }
}

/*
 NOTE:The first call to strtok must pass the C string to tokenize, and subsequent calls must specify NULL as the first argument, which tells the function to continue tokenizing the string you passed in first.
 */
