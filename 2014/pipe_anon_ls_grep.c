//
//  pipe_anon_ls_grep.c
//
//
//  Created by Fabio Nisci on 12/05/14.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main (int argc, char *argv[]){
    int pipefds[2];
    pid_t pid = 0;
    
    if((pipe(pipefds))<0){
        perror("errore pipe\n");
        exit(1);
    }
    
    pid = fork();
    if(pid > 0){
        //processo padre
        close(pipefds[READ]);
        dup2(pipefds[WRITE],WRITE);
        close(pipefds[WRITE]);
        execlp("ls","ls","-r",NULL);
        perror("padre");
    }
    
    else if(pid == 0){
        //processo figlio
        close(pipefds[WRITE]);
        dup2(pipefds[READ],READ);
        close(pipefds[READ]);
        execlp("grep", "grep", argv[1],NULL);
        perror("figlio");
    }
    
    return 0;
}
