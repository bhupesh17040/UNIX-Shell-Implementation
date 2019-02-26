#include<stdio.h>  // THE FOLLOWING CODE TAKES INPUT COMMAND AS STRING AND SEPERATES IT ACROSS  |  , exits if the command is exit
#include <string.h>

int  main()
{
    char sentence[50];
    printf("enter command\n");
    gets(sentence);
    if( 0 == strcmp(sentence, "exit") )
    {
        printf( "equal" );
        exit(0);
    }
    printf("token");
    char *token = strtok(sentence, "|");
    
    
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, "|");
    }
    
    return 0;
    
    
}

