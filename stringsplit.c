#include<stdio.h>  // THE FOLLOWING CODE TAKES INPUT COMMAND AS STRING AND SEPERATES IT ACROSS  |
#include <string.h>

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
    
    int q=0; // variable to count |
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, "|");
        q=q+1;
    }
    
    printf("%d\n", q-1);
    return 0;
    
    
}



