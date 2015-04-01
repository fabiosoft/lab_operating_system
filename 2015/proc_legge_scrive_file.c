/*
Scrivere un programma C che prende come parametri due nomi di file, siano
essi inputF e outputF (controllare che il numero di argomenti passati sia
corretto). Il programma dovrà creare due processi figlio. Il primo figlio, leggerà i
primi 4 caratteri del file inputF (si assume il file contenete almeno una parola),
per scriverli nel file outputF aggiungendo il proprio PID, il secondo figlio
leggerà gli ultimi 4 caratteri di inputF stampandone il contenuto nel file
outputF e aggiungendo il proprio PID. Il padre, aspetterà i figli e scriverà alla
fine del file outputF il proprio PID.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	
	if(argc < 2){
		printf("%s inputF outputF\n", argv[0]);
		return 1;
	}

	pid_t figlio1 = 0, figlio2 = 0;
	char *inputF = argv[1];
	char *outputF = argv[2];

	unlink(inputF);
	int fd = open(inputF, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
	write(fd, "fabio", 5); //scrive parola nel file
	close(fd);

	unlink(outputF);
	int out = open(outputF, O_TRUNC|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
	close(out);

	if((figlio1 = fork())){
		if((figlio2 = fork())){
			//padre
			waitpid(figlio1, NULL, 0);
			waitpid(figlio2, NULL, 0);
			exit(0);
		}else{
			//figlio 2
			int fd = open(inputF, O_RDONLY);
			if(fd > 0){
				lseek(fd,-4,SEEK_END);
				char *buff = malloc(4 * sizeof(char));
				int bytesRead = read(fd, buff, sizeof(buff));
				char *scrivere = malloc(sizeof(buff) + sizeof(int));
				sprintf(scrivere, "%s%d", buff, (int)getpid());

				int out = open(outputF, O_WRONLY|O_APPEND);
				if(out > 0){
					write(out, scrivere, sizeof(scrivere));
				}else{
					printf("Errore apertua outputF: %s\n", outputF);
					perror("outputF");
				}
				close(out);
			}else{
				printf("Errore apertua inputF: %s\n", inputF);
			}
			close(fd);
			exit(0);
		}
	}else{
		//figlio 1
		int fd = open(inputF, O_RDONLY);
		if(fd > 0){
			lseek(fd,0,SEEK_SET);
			char *buff = malloc(4 * sizeof(char));
			int bytesRead = read(fd, buff, sizeof(buff));
			char *scrivere = malloc(sizeof(buff) + sizeof(int));
			sprintf(scrivere, "%s%d", buff, (int)getpid());

			int out = open(outputF, O_WRONLY|O_APPEND);
			if(out > 0){
				write(out, scrivere, sizeof(scrivere));
			}else{
				printf("Errore apertua outputF: %s\n", outputF);
				perror("outputF");
			}
			close(out);
		}else{
			printf("Errore apertua inputF: %s\n", inputF);
		}
		close(fd);
		exit(0);
	}


	return 0;
}