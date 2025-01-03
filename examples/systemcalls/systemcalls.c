#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
 */
bool do_system(const char *cmd)
{
    int ret;
    ret = system(cmd);
    if (ret == -1)
    {
        printf("Failed to create child process.\nError: %s\n", strerror(errno));
        return false;
    }
    else if (WIFEXITED(ret) && WEXITSTATUS(ret) == 127)
    {
        printf("Shell could not be executed, invalid command. Status: %d\n", WEXITSTATUS(ret));
        return false;
    }
    else if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0)
    {
        printf("Command failed to run. Status: %d\nError: %s", WEXITSTATUS(ret), strerror(WEXITSTATUS(ret)));
        return false;
    }
    else if (cmd == NULL)
    {
        if (ret == 0)
            printf("cmd is NULL and shell is not available.\n");
        else
            printf("cmd is NULL and shell is available.\n");
    }
    return true;
}

/**
 * @param count -The numbers of variables passed to the function. The variables are command to execute.
 *   followed by arguments to pass to the command
 *   Since exec() does not perform path expansion, the command to execute needs
 *   to be an absolute path.
 * @param ... - A list of 1 or more arguments after the @param count argument.
 *   The first is always the full path to the command to execute with execv()
 *   The remaining arguments are a list of arguments to pass to the command in execv()
 * @return true if the command @param ... with arguments @param arguments were executed successfully
 *   using the execv() call, false if an error occurred, either in invocation of the
 *   fork, waitpid, or execv() command, or if a non-zero return value was returned
 *   by the command issued in @param arguments with the specified arguments.
 */
bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    for (i = 0; i < count; i++)
    {
        if (command[i][0] != '/' && command[i][0] != '-')
        {
            printf("%s is not an absolute path", command[i]);
            return false;
        }
    }

    fflush(stdout);
    pid_t cpid;
    cpid = fork();
    if (cpid == -1)
    {
        printf("Failed to create child process.\nError: %s\n", strerror(errno));
        return false;
    }
    if (cpid == 0)
    { // child
        int ret;
        ret = execv(command[0], command);
        if (ret == -1)
        {
            printf("execv failed.\nError: %s\n", strerror(errno));
            return false;
        }
    }
    else
    { // parent
        int wstatus;
        pid_t wpid;
        wpid = waitpid(cpid, &wstatus, 0);
        if (wpid == -1)
        {
            printf("waitpid failed.\nError: %s\n", strerror(errno));
            return false;
        }
        if (!WIFEXITED(wstatus) && !WEXITSTATUS(wstatus))
            return false;
    }
    va_end(args);
    return true;
}

/**
 * @param outputfile - The full path to the file to write with command output.
 *   This file will be closed at completion of the function call.
 * All other parameters, see do_exec above
 */
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    int fd;
    fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0)
    {
        printf("Failed to open file");
        return false;
    }

    fflush(stdout);
    pid_t cpid;
    cpid = fork();
    if (cpid == -1)
    {
        printf("Failed to create child process.\nError: %s\n", strerror(errno));
        return false;
    }
    if (cpid == 0)
    { // child
        int ret;
        if (dup2(fd, 1) < 0)
        {
            perror("dup2");
            return false;
        }
        close(fd);

        ret = execv(command[0], command);
        if (ret == -1)
        {
            printf("execv failed.\nError: %s\n", strerror(errno));
            return false;
        }
    }
    else
    { // parent
        int wstatus;
        pid_t wpid;
        wpid = waitpid(cpid, &wstatus, 0);
        if (wpid == -1)
        {
            printf("waitpid failed.\nError: %s\n", strerror(errno));
            return false;
        }
        if (!WIFEXITED(wstatus) && !WEXITSTATUS(wstatus))
            return false;
        close(fd);
    }
    va_end(args);
    return true;
}
