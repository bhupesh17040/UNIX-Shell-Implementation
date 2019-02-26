#include<stdio.h>  // THE FOLLOWING CODE TAKES INPUT COMMAND AS STRING AND SEPERATES IT ACROSS  |
#include <string.h>

int  main()
{
    char sentence[50];
    printf("enter command\n");
    gets(sentence);
    
    char *token = strtok(sentence, "|");
    
    
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, "|");
    }
    
    return 0;
    
    
}
