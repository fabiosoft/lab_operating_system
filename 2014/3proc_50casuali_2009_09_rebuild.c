//
//  3proc_50casuali_2009_09.c
// 	rebuild
//
//
//  Created by Fabio Nisci on  08/09/14. 
//
//

/*
 Si realizzi un programma C in ambiente Linux, che implementi la seguente interazione di processi:
 - il sistema consiste di 4 processi: un processo padre (P1) che provvede alla creazione di 3 processi figli (P11, P12 e P13 ) e di una pipe Pa;
 - il padre P1 attiva ciascun figlio attraverso un segnale SIGUSR2;
 - P1 attende il completamento dei figli;
 - P11 provvede a generare 50 numeri casuali positivi e un messaggio di terminazione (-1), scrivendoli sulla pipe;
 - P12 e P13 prelevano in modo competitivo i numeri dalla pipe e li trascrivono ciascuno su un proprio file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

int main (int argc, char *argv[]){

 	int P11,P12,P13;
 	int pipePA[2];
 	pipe(pipePA);

 	if((P11 = fork())){
 		if((P12 = fork())){
 			if((P13 = fork())){
 				//padre
 				printf("PIDS: %d %d %d\n",P11,P12,P13);
 				waitpid(P11,NULL,0);
 				waitpid(P12,NULL,0);
 				waitpid(P13,NULL,0);
 				printf("Terminato.\n");
 			}else{
 				//P13
 				close(pipePA[1]);
				char *buffer = malloc(1 * sizeof(int));
				int num_letto = -1;
				unlink("P13.txt");
				int fd = open("P13.txt", O_CREAT|O_WRONLY|O_TRUNC, 777);
				if(fd > 0){
					do{
						int bytesRead = read(pipePA[0],buffer,sizeof(int));
						num_letto = atoi(buffer);
						//printf("P12 letto: %d\n", num_letto);
						char *daScrivere = malloc(1 * sizeof(int));
						write(fd, buffer, sizeof(int));
						write(fd, "\n", sizeof(char));
					}while( num_letto >= 0);
				}else{
					printf("Errore creazione file.\n");
				}
				kill(P12, SIGKILL);
				exit(0);
 			}
		}else{
			//P12
			close(pipePA[1]);
			char *buffer = malloc(1 * sizeof(int));
			int num_letto = -1;
			unlink("P12.txt");
			int fd = open("P12.txt", O_CREAT|O_WRONLY|O_TRUNC, 777);
			if(fd > 0){
				do{
					int bytesRead = read(pipePA[0],buffer,sizeof(int));
					num_letto = atoi(buffer);
					//printf("P12 letto: %d\n", num_letto);
					char *daScrivere = malloc(1 * sizeof(int));
					write(fd, buffer, sizeof(int));
					write(fd, "\n", sizeof(char));
				}while( num_letto >= 0);
			}else{
				printf("Errore creazione file.\n");
			}
			kill(P13, SIGKILL);
			exit(0);
		}
	}else{
 		//P11
 		close(pipePA[0]);
 		char *buff = malloc(1 * sizeof(int));
 		for (int i = 0; i < 50; i++){
 			int random_num = 1 + rand() % 10;
 			sprintf(buff, "%d", random_num);
 			write(pipePA[1], buff, sizeof(int));
 			//printf("P11 scritto: %d\n", random_num);
 		}
 		sprintf(buff,"%d",-1);
 		write(pipePA[1],buff, sizeof(int));
 		exit(0);
 	}

 	return 0;
 }