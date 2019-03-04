#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
void redirect_output(char command[] , char fn[]){
    freopen(fn,"w",stdout);
    dup2(fileno(stdout), 1);
    
    
    execlp(command,command,NULL);
    fclose(stdout);
    
    
    
}
void redirect_error(char command[] , char fn[]){
    freopen(fn,"w",stderr);
    dup2(fileno(stderr), STDERR_FILENO);

    
    execlp(command,command,NULL);
    fclose(stderr);
    
    
    
}
void aredirect_output(char command[] , char fn[]){
    freopen(fn,"a",stdout);
    dup2(fileno(stdout), 1);
    
    
    execlp(command,command,NULL);
    fclose(stdout);
    
    
    
}
void redirect_error_to_out(){
    dup2(1,2); // duplicates fd[1] to fd[2]
}

void redirectfromfile(char command[],char fn[])  // command<filename
{
	close(0);
	freopen(fn,"r",stdin);
	int pid;
	pid=fork();
	if(pid==0)
	{
		execlp(command,command,NULL);
	}
	fclose(stdin);
}

/*
* Function to carry out pipelining. It takes a single string as its input,
* which it then splits according to the pipe delimiter "|". An error is thrown
* if a pipe cannot be created or forking fails.
*/
void pipefn (char *a[], char *b[])
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Error creating pipe!");
    }
    int pid = fork();
    if (pid)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        // dup2 lets you choose the file descriptor
        // number that will be assigned and atomically
        // closes and replaces it if it's already taken.
        execv(a[0], a);
    }
    else if (!pid)
    {
        close(fd[1]);
        dup2(fd[0], 0);
        execv(b[0], b);
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
            if(sentence[i+1]=='>'){
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
    printf("%d\n",countappend);
    }
 
 // Will proceed when pipelining would be done completely   
}