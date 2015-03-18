#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **matrice = NULL;
int minimo = 1000;
int rows = 0,cols = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *somma_col(void *col){

	int colonna = *(int *)col;
	printf("T: %d\n COL: %d\n",(int)pthread_self(),colonna);

	for(int rig=0; rig<rows; rig++){
		pthread_mutex_lock(&mutex);
		if( matrice[rig][colonna] < minimo ){
			minimo = matrice[rig][colonna];
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(0);
}



int main(int argc, char *argv[]){
	if(argc < 2){
		printf("USO %s <num_col>\n", argv[0]);
		return -1;
	}

	rows = cols = atoi(argv[1]);

	//allocazione righe
	matrice = malloc( rows * sizeof(int *));
	// costruzione matrice
	for(int row = 0; row < rows; row++){
		//allocazine colonne per ogni riga
		matrice[row] = malloc(cols * sizeof(int));
	    for(int col = 0; col < cols; col++){
	    	//assegnazione valore random
	    	int valore = 1 +  rand() % 10; //numero
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

	pthread_t *threads = malloc(cols * sizeof(pthread_t));

	//passaggio particolare riga
	for (int i=0; i < cols; i++) {
	    int *col=malloc(sizeof(int));
	    *col=i;
	    pthread_create(&threads[i],NULL,somma_col,col);
	}

	for (int i=0; i < cols; i++) {
		pthread_join(threads[i],NULL);
	}

	printf("Il minimo è: %d\n", minimo);



	return 0;
}