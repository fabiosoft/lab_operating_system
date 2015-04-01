/*
 Scrivere un programma che intercetta CTRL‐c ed invece di terminare scrive su stdout “non voglio terminare”
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void sigHandler(int signal){
    printf ("Process %d got a SIGINT\n", getpid());
    printf("non voglio terminare\n");
}

int main(int argc, char *argv[]){
    signal(SIGINT, sigHandler);
    printf("mio PID: %d\n",getpid());

    while(1){
        
    }
    return 0;
}
