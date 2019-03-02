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



