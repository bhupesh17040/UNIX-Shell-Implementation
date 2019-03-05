#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>

void redirect_output(char command[] , char fn[])
{
    freopen(fn,"w",stdout);
    dup2(fileno(stdout), 1);
    execlp(command,command,NULL);
    fclose(stdout);
    
}

void redirect_error(char command[] , char fn[])
{
    freopen(fn,"w",stderr);
    dup2(fileno(stderr), STDERR_FILENO);
    execlp(command,command,NULL);
    fclose(stderr);
    
}

void aredirect_output(char command[] , char fn[])
 {
    freopen(fn,"a",stdout);
    dup2(fileno(stdout), 1);
    execlp(command,command,NULL);
    fclose(stdout);
    }
void redirect_error_to_out()
{
    dup2(1,2); // duplicates fd[1] to fd[2]
}

void redirectfromfile(char command[],char fn[])  // command<filename
{
	close(0);
	freopen(fn,"r",stdin);
	int pid;
	pid=fork();
	if(!pid)
	{
		execlp(command,command,NULL);
	}
	fclose(stdin);
}

/*
* Function to carry out pipelining. It takes two constant pointers to char arrays
* as input. An error is thrown if a pipe cannot be created or forking fails.
* "execvp" is used to execute commands with a variable number of arguments.
*/
void pipefn (char *const a[], char *const b[])
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Error creating pipe!");
    }
    int pid = fork();
    if (pid)
    {
        wait(NULL);
        close(fd[0]);
        dup2(fd[1], 1);// dup2 lets you choose the file descriptor number that will be assigned and atomically closes and replaces it if it's already taken.
        execvp(a[0], a);
    }
    else if (!pid)
    {
        close(fd[1]);
        dup2(fd[0], 0);
        execvp(b[0], b);
    }
    else
    {
        perror("Error forking process!");
    }
}


int  main()
{
    while(1)
    {
        char sentence[50];
        printf("enter command\n");
        gets(sentence);
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
        if (countpipe==0 && countout==0 && countin==0 && countappend==0 && countfd==0) // This is the case when there is an internal command.
        {
            int j=0;
            char *p1=strtok(sentence," ");
            char *a1[3];
            while(p1!=NULL)
            {
                a1[j++]=p1;
                p1=strtok(NULL," ");
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
                    system(sentence);
                }
            }
        }
        else
        {
            int i = 0;
            char *p = strtok(sentence, "|");
            char *array[50];
            while (p != NULL)
            {
                array[i++] = p;
                p = strtok (NULL, "|");
            }
            for (int j = 0; j < i; j++)
            {
                printf("%s\n", array[j]);
            }
        }
    }
 
}