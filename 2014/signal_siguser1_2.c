#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int operazione = 0;

void signalHandler(int signal){
    if(signal == SIGINT){
        exit(0);
    }else{
        operazione = signal;
    }
}
int main(int argc, char *argv[]){
    if(signal(SIGUSR1,signalHandler) == SIG_ERR)
        return EXIT_FAILURE;
    if(signal(SIGUSR2,signalHandler) == SIG_ERR)
        return EXIT_FAILURE;
    if(signal(SIGINT,signalHandler) == SIG_ERR)
        return EXIT_FAILURE;
    
    printf("mio pid: %d\n",getpid());
    
    int a = 3;
    int b = 2;
    
    while(1){
        if (operazione == SIGUSR1) {
            printf("%d + %d = %d\n",a,b,a+b);
        }
        if (operazione == SIGUSR2) {
            printf("%d - %d = %d\n",a,b,a-b);
        }
        operazione = 0;
    }
    return 0;
}
