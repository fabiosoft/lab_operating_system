/*

somma_1000_numeri_rebuild.c

Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria
Pthread, generi tre thread. I primi due thread sommano 1000 numeri generati
casualmente ed ogni volta incrementano un contatore. 
Il terzo thread attende che il contatore incrementato dai 
due thread raggiunga un valore limite fornito da riga di comando,
notifica l’avvenuta condizione e termina. Utilizzare le variabili condizione.

*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int limite = 650000;
int contatore = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condizione = PTHREAD_COND_INITIALIZER;

void *somma (){
	printf("T somma: %d partito\n",(int)pthread_self());
	int sum = 0;
	for(int i = 0; i < 1000; i++){
		int generato = 1 + rand() % 10;
		sum = sum + generato;

		pthread_mutex_lock(&mutex);
		//printf("Contatore: %d\n", contatore);
		contatore++;
		pthread_mutex_unlock(&mutex);

		if(contatore >= limite){
			pthread_cond_signal(&condizione);
			pthread_exit(NULL);
		}
	}
	return NULL;
}

void *verifica(){
	printf("T verifica: %d partito\n",(int)pthread_self());
	pthread_cond_wait(&condizione, &mutex);
	printf("Limite di %d raggiunto.\n", limite);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t threads[3];

	if(argc < 2){
		printf("Devi fornire valore limite.\n");
		return 1;
	}

	limite = atoi(argv[1]);

 	pthread_create(&threads[0],NULL,somma,NULL);
 	pthread_create(&threads[1],NULL,somma,NULL);
 	pthread_create(&threads[2],NULL,verifica,NULL);
	
	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);
	pthread_join(threads[2],NULL);


	return 0;
}
