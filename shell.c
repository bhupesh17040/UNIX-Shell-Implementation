#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>

int lol;
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
* Function to carry out pipelining. It takes a commands array and its length
* as input. An error is thrown if a pipe cannot be created or forking fails.
* "execvp" is used to execute commands with a variable number of arguments.
*/
void pipefn (char *const cmds[], int lenCmds)
{
    // for (int j = 0; j < lenCmds; j++)
    //         {
    //             printf("%s\n", cmds[j]);
    //         }
    //         printf("%d",lenCmds);
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
            char *temp[] = {cmds[i], NULL};
            execvp(cmds[i], temp);
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
                char *temp[] = {cmds[i+1], NULL};
                execvp(cmds[i+1], temp);
            }
        }
    }
    

}

int  main()
{
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
            char *a1[3];
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
                    system(sentence);
                }
            }
        }
        else
        {
            int i = 0;
            char *p = strtok(sentence, "| \n");
            char *array[50];
            while (p != NULL)
            {
                array[i++] = p;
                p = strtok (NULL, "| \n");
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