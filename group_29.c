/*
ASSIGNMENT 2
GROUP ID-29
GROUP MEMBERS-
NAME-BHUPESH SINGH KAINTH
ROLL NO.-2017040
NAME- SHLOAK AGGARWAL
ROLL NO.-2017107
NAME-SHASHWAT JAIN
ROLL NO.-2017103
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include<signal.h>

/*
* redirect stdout to file “filename”. If the file does not exist create one,
* otherwise, overwrite the existing file
*/
void redirectOutputToFile (char *file)
{
    int o = open(file, O_CREAT|O_TRUNC|O_WRONLY, 0644);
    dup2(o, STDOUT_FILENO);
    close(o);
}

/*
* redirect stderr to filename
*/
void redirectErrorToFile (char *file)
{
    int o = open(file, O_CREAT|O_TRUNC|O_WRONLY, 0644);
    dup2(o, STDERR_FILENO);
    close(o);
}

/*
* If the file already exists, appends the stdout output, otherwise, creates a new file.
*/
void appendOutputToFile (char *file)
{
    int o = open(file, O_CREAT|O_APPEND|O_WRONLY, 0644);
    dup2(o, STDOUT_FILENO);
    close(o);
}

/*
* If the file already exists, appends the stderr output, otherwise, creates a new file.
*/
void appendErrorToFile (char *file)
{
    int o = open(file, O_CREAT|O_APPEND|O_WRONLY, 0644);
    dup2(o, STDERR_FILENO);
    close(o);
}

/*
* Redirects stderr to stdout
*/
void redirectErrorToOutput ()
{
    dup2(STDOUT_FILENO, STDERR_FILENO);
}

/*
* use file descriptor 0 (stdin) for filename. If command tries to read from
* stdin, effectively it will read from filename.
*/
void redirectFileToInput (char *file)
{
    int o = open(file, O_RDONLY);
    dup2(o, STDIN_FILENO);
    close(o);
}

/*
*
*/
int checkRedirection (char *p)
{
    int r = 0;
    for(int i = 0; i < strlen(p); i++)
    {
        if (p[i] == '1')
        {
            if (p[i+1] == '>')
            {
                if (p[i+2] == '>')
                {
                    appendOutputToFile(p+i+3);
                }
                else
                {
                    redirectOutputToFile(p+i+2);
                }
                r = 1;
                break;
            }
        }
        else if (p[i] == '2')
        {
            if (p[i+1] == '>')
            {
                if (p[i+2] == '>')
                {
                    appendErrorToFile(p+i+3);
                }
                else if (p[i+2] == '&')
                {
                    redirectErrorToOutput();
                }
                else
                {
                    redirectErrorToFile(p+i+2);
                }
                r = 1;
                break;
            }
        }
        else if (p[i] == '>')
        {
            if (p[i+1] == '>')
            {
                appendOutputToFile(p+i+2);
            }
            else
            {
                redirectOutputToFile(p+i+1);
            }
            r = 1;
            break;
        }
        else if (p[i] == '<')
        {
            redirectFileToInput(p+i+1);
            r = 1;
            break;
        }
    }
    return r;
}

/*
* Function to carry out pipelining. It takes a commands array and its length
* as input. An error is thrown if a pipe cannot be created or forking fails.
* "execvp" is used to execute commands with a variable number of arguments.
*/
void pipefn (char *const cmds[], int lenCmds)
{
    int fd[2];
    int pid;
    int outputStorage = 0;
    for(int i = 0; i < lenCmds-1; i++)
    {
        if (pipe(fd) == -1)
        {
            perror("Error creating pipe!");
        }
        pid = fork();
        if (pid == -1)
        {
            perror("Error forking");
        }
        else if (pid == 0)
        {
            close(fd[0]);
            dup2(outputStorage, 0);
            dup2(fd[1], 1);
            // dup2 lets you choose the file descriptor number that will be
            // assigned and atomically closes and replaces it if it's already taken.
            int j = 0;
            char *p = strtok(cmds[i], " ");
            char *temp[10];
            while (p != NULL)
            {
                int r = checkRedirection(p);
                if (r == 0)
                {
                    temp[j++] = p;
                }
                p = strtok (NULL, " ");
            }
            temp[j] = NULL;
            execvp(temp[0], temp);
        }
        else
        {
            if (i < lenCmds-2)
            {
                wait(NULL);
                close(fd[1]);
                outputStorage = fd[0];
            }
            else
            {
                wait(NULL);
                close(fd[1]);
                dup2(fd[0], 0);
                int k = 0;
                char *p = strtok(cmds[i+1], " ");
                char *temp[10];
                while (p != NULL)
                {
                    int s = checkRedirection(p);
                    if (s == 0)
                    {
                        temp[k++] = p;
                    }
                    p = strtok (NULL, " ");
                }
                temp[k] = NULL;
                execvp(cmds[i+1], temp);
            }
        }
    }
}

/*
* Trims provided string to remove leading and trailing whitespace.
* Function referred from: cboard.cprogramming.com/c-programming/31839-trim-string-function-code.html
*/
char *trim (char *cmd)
{
    int a = strlen ( cmd ) - 1;
    while ( isspace ( cmd[a] ) && a >= 0 )
    {
        a--;
    }
    int b = 0;
    while ( isspace ( cmd[b] ) && cmd[b] != '\0' )
    {
        b++;
    }
    int c = 0;
    while ( b <= a )
    {
        cmd[c++] = cmd[b++];
    }
    cmd[c] = '\0';
    return cmd;
}

int  main ()
{
    signal(SIGINT,SIG_IGN);  /* Handles SIGINT i.e Ctrl+C interrupt.Kills the currently executing process,
                                and starts waiting for an input command. */
    while(1)
    {
        char sentence[100];
        printf("Shell$ ");
        fgets(sentence, 100, stdin);
        if(strcmp(sentence,"exit")==0)
        {
            exit(0);
        }
        int countpipe=0;
        int countout=0;
        int countin=0;
        int countappend=0;
        int countfd=0;
        for(int i=0;i<strlen(sentence);++i)
        {
            if(sentence[i]=='|')
            {
                countpipe=countpipe+1;
            }
            else if(sentence[i]=='<')
            {
                countout=countout+1;
            }
            else if(sentence[i]=='>')
            {
                if(sentence[i+1]=='>')
                {
                    countappend=countappend+1;
                }
                else if(sentence[i+1]=='&')
                {
                    countfd=countfd+1;
                }
                else
                {countin=countin+1;
                }
            }
        }
        // This is the case when there is an internal command.
        if (countpipe==0 && countout==0 && countin==0 && countappend==0 && countfd==0)
        {
            int j=0;
            char *p1=strtok(sentence," \n");
            char *a1[10];
            while(p1!=NULL)
            {
                a1[j++]=p1;
                p1=strtok(NULL," \n");
            }
            for(int y=0;y<j;y++)
            {
                if(strcmp(a1[y],"cd")==0)  // Implementing cd on my own.
                {
                    char e[100];
                    chdir(a1[y+1]);
                    printf("%s \n",getcwd(e,100));
                }
                else
                {
                    char *temp[] = {sentence, NULL};
                    int pid = fork();
                    if (pid == 0)
                    {
                        execvp(temp[0], temp);
                    }
                    else
                    {
                        wait(NULL);
                    }
                }
            }
        }
        else if (countpipe == 0)
        {
            int pid = fork();
            if (pid == 0)
            {
                int x = 0;
                char *y = strtok(sentence, " ");
                char *arr[10];
                while (y != NULL)
                {
                    int r = checkRedirection(trim(y));
                    if (r == 0)
                    {
                        arr[x++] = trim(y);
                    }
                    y = strtok (NULL, " ");
                }
                arr[x] = NULL;
                execvp(arr[0], arr);
            }
            else
            {
                wait(NULL);
            }
        }
        else
        {
            int i = 0;
            char *p = strtok(sentence, "|");
            char *array[50];
            while (p != NULL)
            {
                array[i++] = trim(p);
                p = strtok (NULL, "|");
            }
            int pid = fork();
            if (pid == 0)
            {
                pipefn(array, i);
            }
            else
            {
                wait(NULL);
            }
        }
    }
 
}