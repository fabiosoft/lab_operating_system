/*

10/09/2014

Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, 
crei una matrice di interi di dimensioni n x n con n numero dispari fornito da riga di comando,
che provveda a sommare in modo concorrente con quattro thread, gli elementi delle due diagonali,
della riga centrale e della colonna centrale della matrice e ne determini il massimo da
assegnare ad un'opportuna variabile.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_THREADS 4

int **matrice = NULL;
int rows = 0, cols = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int maggiore = 0;
int running_threads = N_THREADS-1;

void *calcola( void *arg){

	int somma = 0;
	int pos = *(int * )arg;
	printf("\n\nPosizione %d\n", pos);
	if(pos == 0){
		for (int row = 0; row < rows ; row++) {
    		somma = somma + matrice[row][row];
	    }
	    printf("Diag sx = %d\n",somma);
	}
	if(pos == 1){
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				if(row + col == cols-1){
					// printf("matrice[%d][%d] = %d\n", row,col,matrice[row][col]);
					somma = somma + matrice[row][col];
				}
			}
		}
		printf("Diag dx = %d\n",somma);
	}
	
	if(pos == 2){
		for (int col = 0; col < cols; col++) {
			somma = somma + matrice[rows/2][col];
		}
		printf("Riga centrale %d\n",somma);
	}
	if(pos == 3){
		for (int row = 0; row < rows; row++) {
				somma = somma + matrice[row][cols/2];
			}
		printf("Col centrale = %d\n",somma);
	}

	pthread_mutex_lock(&mutex);
	if(somma > maggiore){
		maggiore = somma;
	}
	running_threads--;
	pthread_mutex_unlock(&mutex);

	

	return NULL;
}

int main(int argc, char *argv[]){
	if(argc < 2 || (atoi(argv[1]) % 2 == 0)){
		printf("Devi inserire la dimensione della matrice dispari.\n");
		return 1;
	}
	rows = atoi(argv[1]);
	cols = atoi(argv[1]);

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
	        int valore = 1+rand()%10;
	        printf("%d \t",matrice[row][col]);  
	    }
	    printf("\n");
	}


	pthread_t *threads = malloc(N_THREADS * sizeof(pthread_t));
	for (int i = 0; i < N_THREADS; i++){
		int *pos=malloc(sizeof(int));
    	*pos=i;
		pthread_create(&threads[i],NULL,calcola,pos);
	}
	
	for (int i = 0; i < N_THREADS; i++){
		pthread_join(threads[i],NULL);
	}

	while(running_threads > 0){
		sleep(1);
	}
	printf("\n\nMaggiore = %d\n", maggiore);




	return 0;
}