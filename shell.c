#include<stdio.h>  
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
    char sentence[50];
    printf("enter command\n");
    gets(sentence);
    if( 0 == strcmp(sentence, "exit") )
    {
        exit(0);
    }
    char *token = strtok(sentence, "|");
    const char *array[2]; // array for inputs

    int q=0; // variable to count |
    while (token != NULL)
    {
        array[q]=token;
        //printf("%s\n", token);
        token = strtok(NULL, "|");
        
        //printf("%s\n", a[q]);
        q=q+1;
    }
    
    //printf("%d\n", q-1);
    return 0;
    
    
}



