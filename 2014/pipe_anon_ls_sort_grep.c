/**
 • Esercizio: scrivere un programma che esegua il comando di shell “ls | sort | grep <pat>” con tre processi distinti
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]){
	int fdpipe1[2], fdpipe2[2];
	pid_t figlioA, figlioB;
    
    if(argc < 2){
        printf("Usage: %s <grep pattern>\n",argv[0]);
        exit(1);
    }
    
    if((pipe(fdpipe1)) < 0){
        fprintf(stderr, "Errore creazione pipe 1: %s\n", strerror(errno));
        exit(errno);
    }
    
    switch(figlioA = fork()){
        case -1:
            fprintf(stderr, "Errore creazione figlio A: %s\n", strerror(errno));
            exit(errno);
            break;
        case 0:
            close(fdpipe1[0]);
            dup2(fdpipe1[1],1);
            close(fdpipe1[1]);

            
            if((pipe(fdpipe2)) < 0){
                fprintf(stderr, "Errore creazione pipe 1: %s\n", strerror(errno));
                exit(errno);
            }
            
            switch(figlioB = fork()){
                case -1:
                    fprintf(stderr, "Errore creazione figlio B: %s\n", strerror(errno));
                    exit(errno);
                    break;
                case 0:
                    close(fdpipe2[0]);
                    dup2(fdpipe2[1],1);
                    close(fdpipe2[1]);
                    execlp("ls","ls",NULL);
                    break;
                default:
                    dup2(fdpipe2[0],0);
                    close(fdpipe2[1]);
                    waitpid(figlioB,NULL,0);
                    execlp("sort","sort",NULL);
            }
            
            break;
            
        default:
            close(fdpipe1[1]);
            dup2(fdpipe1[0],0);
            close(fdpipe1[0]);
            waitpid(figlioA,NULL,0);
            execlp("grep","grep",argv[1],NULL);
    }
    
    
    return 0;
}