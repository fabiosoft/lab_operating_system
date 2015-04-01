#import <stdlib.h>
#import <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	if(argc < 2){
		printf("USAGE: %s <directory path>\n",argv[0]);
		return 1;
	}

	struct dirent *dp;
	DIR *dirp = opendir(argv[1]);
	if (dirp == NULL)
	       return 1;
	while ((dp = readdir(dirp)) != NULL) {
			if(dp->d_type == DT_LNK){
				printf("Link: %s\n", dp->d_name);
				char *filepath = malloc(2 + dp->d_namlen + strlen(argv[1]) * sizeof(char));
				strcat(filepath,"./");
				strcat(filepath,argv[1]);
				strcat(filepath,dp->d_name);
				struct stat *link;
				stat(filepath,link);

				int datfile = open(filepath,O_RDONLY);
				char *buf = malloc(1 * sizeof(char));
				ssize_t bytesread = 0;
				pid_t pid_figlio = 0;
				int status;

				for(int i = 0; i < 20; i++){
					bytesread = read(datfile,buf,1);
					pid_figlio = fork();
					if(pid_figlio == 0){
						pid_t mio_pid = getpid();
						if(mio_pid % 3 != 0){
							//pid non multiplo di 3
							sleep(1);
							printf("%s\n",buf);
							printf("Arrivederci\n");
							_exit(0);
						}else{
							//pid multiplo di 3
							raise(SIGKILL);
						}
					}

					waitpid(pid_figlio, &status, 0);
					if(WIFEXITED(status)){
						printf("%d Figlio %d - terminazione normale %d\n", i+1, pid_figlio, status);
					}
					if(WIFSIGNALED(status)){
						printf("%d Figlio %d - terminazione segnale %d\n", i+1, pid_figlio, status);
					}
					

				}
				printf("\n");
			}
	}
	closedir(dirp);
	
	return 0;
}