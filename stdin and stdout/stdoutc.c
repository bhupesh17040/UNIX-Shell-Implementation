//redirect stdout to file “filename”. If the file does not exist create one,
//otherwise, overwrite the existing file
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
void redirect( command ,char fn[]){
    // this is written to redir.txt
    freopen(fn,"w",stdout);
    
    dup2(fileno(stdin), STDOUT_FILENO);
    
    execlp(command,command,NULL);
    fclose(stdout);
}
int main(void)


{
    execlp(ls,ls,NULL);
    redirect("ls","redir.txt");
}
