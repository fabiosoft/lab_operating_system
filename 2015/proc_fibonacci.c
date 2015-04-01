/*
• Scrivere un programma C in cui un processo crea un processo figlio 
•  Il processo figlio calcola la sequenza di Fibonacci di ordine n (n<=12). Quando termina restituisce il valore calcolato come codice di terminazione 

•  Il padre Attende la terminazione del figlio ed esamina lo stato di terminazione 
• Se lo stato di terminazione è rela/vo ad una terminazione con successo e il codice di terminazione è un valore minore di 50 
•  Crea un secondo figlio che esegue il comando ls –al a.out 

•  Attende il secondo figlio, stampa un messaggio e termina 
• Altrimenti stampa un messaggio e termina 
*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int fibonacci(int i){
	if (i < 0) return -1; /* F(i) non e' definito per interi i negativi! */

	if (i == 0) return 0;
	else if (i == 1) return 1;
	else return fibonacci(i-1) + fibonacci(i-2);
}

int main(int argc, char *argv[]){
	pid_t pid = 0;
	int n = 1;
	pid = fork();
	if(pid == 0){
		int fib = fibonacci(n);
		printf("Fib = %d\n", fib);
		exit(fib);
	}
	int status;
	waitpid(pid, &status, 0);
	printf("Cod figlio = %d\n", WIFEXITED(status));

	if(WIFEXITED(status) > 0 && status < 50){
		pid = fork();
		if(pid == 0){
			execlp("ls", "-al", "es2", NULL);
			exit(1);
		}
		waitpid(pid,NULL,0);
		printf("Figlio terminato, termino.\n");
		_exit(0);
	}else{
		printf("Termino.\n");
		exit(0);
	}
	

	return 0;
}