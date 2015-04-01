#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	pid_t pid =0;
	pid=fork();
	if (pid>0){
		printf("In attesa\n");
		waitpid(pid, NULL, 0);
		printf("Nuovo figlio\n");
		pid=fork();
		if (pid==0) {
			sleep(5);
			_exit(0);
		}
		waitpid(pid, NULL, 0);
		printf("Pid:%d\n",pid);
	}else {
		execlp("ls", "ls","-l",NULL);
		_exit(0);
	}
	return 0;
}