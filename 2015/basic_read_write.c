/*
• Scrivere un programma C che:
• Prende in input coppie di interi utilizzando la system call read
• Calcola la somma degli interi
• Stampa a video il risultato utilizzando la write
• Termina quando il primo input e' ‐1
• Assumere che gli interi consistano di una sola cifra
*/



#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

	int x,y;
	char *buf = malloc(sizeof(int));
	printf("Inserisci il primo intero\n" );
	read(0,buf,sizeof(int));
	x= atoi(buf);
	printf("Inserisci il secondo intero\n" );
	read(0,buf,sizeof(int));
	y= atoi(buf);
	int z= x+y;
	sprintf(buf,"%d",z);
	write(1,buf,sizeof(int));
	return 0;
}