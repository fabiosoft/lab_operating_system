/*

13/09/2014

Scrivere un programma C in ambiente Linux che, ricevuto il nome di un file, 
lo cerchi all’interno della directory corrente e se esso è un file regolare
di dimensione maggiore di 1Kb ne faccia, eventualmente, il taglia e incolla
nella directory puntata da un link simbolico presente nella directory corrente.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


int main(int argc, char *argv[]){

	if(argc < 2){
		printf("Inserire nome file\n");
		return 1;
	}

	char *nomeFile = malloc(strlen(argv[1]) * sizeof(char));
	strcpy(nomeFile, argv[1]);

	printf("cerco: %s\n", nomeFile);

	struct dirent *dp;
	struct stat fileTrovato;
	DIR *dirp = opendir(".");

	int len = strlen(nomeFile);
	while ((dp = readdir(dirp)) != NULL) {
	       if (dp->d_namlen == len && strcmp(dp->d_name, nomeFile) == 0) {
	               printf("Trovato.\n");
	               lstat(nomeFile,&fileTrovato);
	               if(S_ISREG(fileTrovato.st_mode)){
               			printf("File regolare.\n");
               			if((int)fileTrovato.st_size > 1000){
               				printf("File > 1K\n");
               				
               				int origine = open(nomeFile, O_RDONLY);
               				char *nome_link = "link";
               				char *destFile = malloc(1+strlen(nome_link)+strlen(nomeFile) * sizeof(char));
							strcpy(destFile, "link/");
							strcat(destFile, nomeFile);
							unlink(destFile);
       						int destinazione = open(destFile, O_CREAT|O_TRUNC|O_WRONLY,777);

       				        printf("Inizio la copia\nda: %s a %s\n",nomeFile,destFile);

               				char *buf = malloc(1 * sizeof(char));
               				int bytesRead = 0;
               				if(origine > 0 && destinazione > 0){
               					do{
               						bytesRead = read(origine, buf, sizeof(char));
               						// printf("Letto: %s\n", buf);
               						write(destinazione, buf, sizeof(char));
               						// printf("Scritto: %s\n", buf);
               					}while(bytesRead > 0);
               					close(destinazione);
               					close(origine);
               				}
               				unlink(origine);//taglia file.
               				printf("Fine copia.\n");
               			}
	               }
	       }
	}
	closedir(dirp);

	return 0;
}