#include <stdio.h>
#include <syslog.h>

int main(int argc, char* argv[])
{
    openlog(NULL, 0, LOG_USER);
    
    if(argc != 3)
    {
        syslog(LOG_ERR, "Invalid number of input arguments: %d", argc);
        closelog();
        return 1;
    }

    const char *writeFile = argv[1];
    const char *writeStr = argv[2];

    syslog(LOG_DEBUG, "File to write into: %s", writeFile);
    syslog(LOG_DEBUG, "String to write: %s", writeStr);

    FILE *writeFileFd = fopen(writeFile, "w");
    if(writeFileFd == NULL)
    {
        syslog(LOG_ERR, "Failed to open file: %s", writeFile);
        closelog();
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", writeStr, writeFile);
    if(fprintf(writeFileFd, "%s", writeStr) < 0)
    {
        syslog(LOG_ERR, "Error writing to file: %s", writeFile);
        fclose(writeFileFd);
        closelog();
        return 1;
    }

    fclose(writeFileFd);

    syslog(LOG_DEBUG, "Successfully written %s to %s", writeStr, writeFile);
    closelog();
    return 0;
}

