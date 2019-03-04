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
void pipefn (char str[])
{
    char *const *argv1;
    char *const *argv2;

    argv1 = strtok(str, "|");              // store command left of pipe in arg1
    argv2 = strtok(NULL, "|");             // store command right of pipe in arg2

    // argv1[1] = 0;
    // argv1[1] = 0;

    // char *bin = "/bin/";                        // string to store prefix of paths

    // allocate enough memory for prefix path, command and null character at the end
    // char *cmd1 = malloc( strlen(argv1[0]) + 6 );    // variable for left command
    // char *cmd2 = malloc( strlen(argv2[0]) + 6 );    // variable for right command

    // append prefix path to both cmd1 and cmd2
    // strcat(cmd1, bin);
    // strcat(cmd2, bin);

    // append command from arg1 and arg2 to cmd 1 and cmd 2, respectively, to complete paths
    // strcat(cmd1, argv1[0]);
    // strcat(cmd2, argv2[0]);

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Error creating pipe!");
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Error forking process!");
    }
    else if (pid == 1)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        // dup2 lets you choose the file descriptor
        // number that will be assigned and atomically
        // closes and replaces it if it's already taken.
        execlp(argv1, argv1, NULL);
    }
    else if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        execv(argv2, argv2);
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



