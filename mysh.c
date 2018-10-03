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

  //do {
      /*
       * Print the prompt
       */

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
    do{
        printf("mysh$ ");
        getline(&line,&len,stdin);
        
        //Strip newline
        if('\n' == line[strlen(line)-1]){
            line[strlen(line)-1] = '\0';
        }
        
        //TOKENIZE
        char *token;
        token = strtok(line," ");
        if(strcmp(token,"exit")==0){
            
            //wait for background processes
            return 0;
        }else if(strcmp(token,"jobs")==0){
            printf("<Print out jobs>");
            return 0;
        }else if(strcmp(token,"history")==0){
            //HISTORY
            return 0;
        }else if (strcmp(token,"fg")==0){
            //TESTING PIDS
            //            pid_t c_pid = 0;
            //            int status = 0;
            //            char *binary = NULL;
            //            char **args = NULL;
            //            binary = strdup("ls"); args = (char **)malloc(
            //                                                          sizeof(char *) * 3); args[0] = strdup(binary);
            //            args[1] = strdup("."); args[2] = NULL;
            //            /* fork a child process */
            //            c_pid = fork();
            //
            //            /* Check for an error */
            //            if( c_pid < 0 ) {
            //                fprintf(stderr,
            //                        "Error: Fork failed!\n");
            //                return -1; }
            //            /* Check if child */
            //            else if( c_pid == 0 ) {
            //                execvp(binary, args);
            //                /* exec does not return on success.
            //                 * If we are here then error out */
            //                fprintf(stderr,
            //                        "Error: Exec failed!\n");
            //                exit(-1);
            //            }
            //            /* Check if parent */
            //            else {
            //                waitpid(c_pid, &status, 0);
            //                printf("Child finished!\n"); }
            return 0;
        }else if(strcmp(token,"wait")==0){
            printf("Wait");
        }
        while( token != NULL ){
            printf("Token: %s\n",token);
            /*
             NOTE:The first call to strtok must pass the C string to tokenize, and subsequent calls must specify NULL as the first argument, which tells the function to continue tokenizing the string you passed in first.
             */
            token = strtok (NULL, " ");
        }
        //EXECUTE COMMAND
    }while(1==1);
    /*
     * Read stdin, break out of loop if Ctrl-D
     */


    /* Strip off the newline */


    /*
     * Parse and execute the command
     */

  //} while( 1/* end condition */);

  /*
   * Cleanup
   */

  return 0;
}

/*
 * You will want one or more helper functions for parsing the commands
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job) {

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
