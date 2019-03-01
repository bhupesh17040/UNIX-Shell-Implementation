#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
int main(void)

{
        if (freopen("redir.txt", "r", stdin) == NULL)
        {
            perror("freopen() failed");
            return EXIT_FAILURE;
        }
        // this is written to redir.txt
       
        dup2(fileno(stdin), STDIN_FILENO);
        
        execlp("wc","wc",NULL);
        fclose(stdin);
    }






