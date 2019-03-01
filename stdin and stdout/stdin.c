//command < filename
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>
void redirect(char command[] , char fn[]){
    // this is written to redir.txt
    freopen(fn,"r",stdin);
    dup2(fileno(stdin), STDIN_FILENO);
    
    execlp(command,command,NULL);
    fclose(stdin);
}
int main(void)


{
    redirect("wc","redir.txt");
    }






