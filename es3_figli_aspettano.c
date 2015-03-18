#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main (int argc, char *argv[]){
	int pid = 0;
	int status = 0;
	pid = fork();
	if(pid!=0){
		printf("In attesa\n");
		waitpid(pid,&status,0);
        
		if (WIFEXITED(status)) {
			printf("nuovo figlio\n");
			pid = fork();
			if(pid!=0){
				waitpid(pid,&status,0);
				if (WIFEXITED(status)) {
					printf("terminato pid %d\n", pid);
				}
			}else if(pid == 0){
				sleep(5);
				printf("un messaggio\n");
				exit(0);
			}
			
		}
	}else if(pid == 0){
		execlp("ls","ls","-l",NULL);
        exit(0);
    }
	
    exit(0);
}
