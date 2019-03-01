//command < filename
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
void redirect(char command[] , char fn[]){
    // this is written to redir.txt
    freopen(fn,"w",stdout);
    dup2(fileno(stdout), STDOUT_FILENO);
    
    execlp(command,command,NULL);
    fclose(stdout);
}
int main(void)


{
    redirect("ls","redir.txt");
}
