#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
int main(void)

{

   // char a[]=execlp("ls","ls",NULL);
      //  puts("sh");
    if (freopen("redir.txt", "w", stdout) == NULL)
    {
        perror("freopen() failed");
        return EXIT_FAILURE;
    }
    puts(execlp("ls","ls",NULL)); // this is written to redir.txt
    

    fclose(stdout);
    
     
    }
   



