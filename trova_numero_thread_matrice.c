/*

13 ottobre 2014
Fabio Nisci

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **matrice = NULL;
int rows = 0, cols = 0;
int x = 0;
int contatore = 0;

struct posizione{
	int individuato;
	int riga;
	int colonna;
};

struct posizione *trovati = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *cerca(void *r){

	int riga = *(int *)r;
	printf("T: %d R: %d\n", (int)pthread_self(),riga);

	for(int i = 0; i < cols; i++){
		pthread_mutex_lock(&mutex);
		if( matrice[riga][i] == x ){
			contatore++;
			trovati[0].riga = riga;
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(0);
}


void *aspetta(){
	pthread_cond_wait(&cond, &mutex);
	printf("TA: %d\n",(int)pthread_self());
	//printf("trovati[0].riga = %d\n", trovati[0].riga);
	printf("contatore = %d\n", contatore);
	pthread_exit(0);	
}


int main(int argc, char *argv[]){

	if(argc < 3){
		printf("USAGE: %s <m> <n> <x>\n", argv[0]);
		return 1;
	}

	rows = atoi(argv[1]); // m
	cols = atoi(argv[2]); // n
	x = atoi(argv[3]); // x

	trovati = malloc(sizeof(struct posizione));


	//allocazione righe
	matrice = malloc( rows * sizeof(int *));
	// costruzione matrice
	for(int row = 0; row < rows; row++){
		//allocazine colonne per ogni riga
		matrice[row] = malloc(cols * sizeof(int));
	    for(int col = 0; col < cols; col++){
	    	//assegnazione valore random
	    	int valore = 1 +  rand() % 5; //numero
	        matrice[row][col] = valore;
	    }
	}

	//stampa matrice
	for (int row = 0; row < rows ; row++) {
	    for (int col = 0; col < cols; col++) {
	        printf("%d \t",matrice[row][col]);  
	    }
	    printf("\n");
	}


	pthread_t *threads = malloc(1+rows * sizeof(pthread_t));
	//passaggio particolare riga
	for (int i=0; i < rows; i++) {
	    int *row=malloc(sizeof(int));
	    *row=i;
	    pthread_create(&threads[i],NULL,cerca,row);
	}

	pthread_create(&threads[rows+1],NULL,aspetta,NULL);

	for (int i=0; i < rows+1; i++) {
		pthread_join(threads[i],NULL);
	}


	return 0;
}