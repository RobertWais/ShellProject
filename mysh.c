/*
 * Robert Wais
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"
#include <stdio.h>
#include <string.h>

#define LINELEN 256

int main(int argc, char * argv[]) {

    /*
     * Parse Command line arguments to check if this is an interactive or batch
     * mode run.
     */
    
    //( 0 != (ret = parse_args_main(argc, argv)) )
    if( 0 != (parse_args_main(argc, argv)) ) {
        fprintf(stderr, "Error: Invalid command line!\n");
        return -1;
    }

    /*
     * If in batch mode then process all batch files
     */
    if( TRUE == is_batch) {
        if( TRUE == is_debug ) {
            printf("Batch Mode!\n");
        }

//        ( 0 != (ret = batch_mode()) )
        if( 0 != (batch_mode()) ) {
            fprintf(stderr, "Error: Batch mode returned a failure!\n");
        }
    }
    /*
     * Otherwise proceed in interactive mode
     */
    else if( FALSE == is_batch ) {
        if( TRUE == is_debug ) {
            printf("Interactive Mode!\n");
        }

        //(ret = interactive_mode())
        if( 0 != (interactive_mode()) ) {
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

int parse_args_main(int argc, char **argv)
{

    //INTERACTIVE MODE
    if(argc<2){
    
    printf("Argc: %d\n",argc);
    printf("Agv: %s\n",argv[0]);
        
    //BATCH MODE
    }else{
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

int batch_mode(void)
{
    int i;
//    for(i = 0; i < num_batch_files ; i++){
//        printf("Batch file: %s\n",batch_files[i]);
//
//        FILE *fptr;
//        if ((fptr = fopen(batch_files[i],"r")) == NULL){
//            printf("Error! opening file");
//
//            //BREAK FROM EVERYTHING
//            return(-1);
//        }
//
//        //READ LINES
//        char *check;
//        fscanf(fptr,"%s",check);
//        printf("Word: %s",check);
//
//    }
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

int interactive_mode(void)
{

    //do {
        /*
         * Print the prompt
         */
        
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

int launch_job(job_t * loc_job)
{

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

int builtin_exit(void)
{

    return 0;
}

int builtin_jobs(void)
{

    return 0;
}

int builtin_history(void)
{

    return 0;
}

int builtin_wait(void)
{

    return 0;
}

int builtin_fg(void)
{

    return 0;
}
