#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * @fn write_to_file
 * @brief Write to a file specified by passed arguments through
 * the CLI. This also writes to syslog.
 * @return Int if file write is successful or fails
*/

int write_to_file(char *string, char *filename)
{
    //initialize exit status int
    int exit_status = 0;
    //open file with write argument
    FILE *fp = fopen(filename, "w");
    
    //check if fp is not null
    if(!fp)
    {
        printf("Could not open file.");
        syslog(LOG_ERR, "Could not open file %s", filename);
        exit_status = EXIT_FAILURE;
        return exit_status;
    }

    //write string to target file
    fprintf(fp, "%s", string);

    //check file error
    if(ferror(fp))
    {
        syslog(LOG_ERR, "I/O error when reading.");
        exit_status = EXIT_FAILURE;
    }else if(feof(fp))
    { //check if at end of file might be redundant
        syslog(LOG_INFO, "File write success.");
        exit_status = EXIT_SUCCESS;
    }
    //close file
    fclose(fp);
    //exit    
    return exit_status;
}

int main(int argc, char *argv[])
{
    //initialize input vars
    char *writestr = argv[1];
    char *filename = argv[2];

    //initialize syslog
    openlog(NULL, 0, LOG_USER);

    /*check for correct arguments, the exec file 
    * is counted as an argument
    */
    if(argc == 3)
    {
        syslog(LOG_DEBUG, "Writing %s to file %s", writestr, filename);
        write_to_file(filename, writestr);
    } else {
        return 1;
    }
    /**
     * close syslog
    */
    closelog();

    return 0;
}