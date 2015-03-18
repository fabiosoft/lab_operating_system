/*
Scrivere un programma C “scrivi.c” che utilizzando la
funzione primitiva “write”, scriva in un file “alfabeto.txt” la
seguente stringa di caratteri: “ABCDEFGHILMNOPQRSTUVZ”
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
	
	char *nomefile = "alfabeto.txt";
	unlink(nomefile);

	int fd = open(nomefile, O_CREAT | O_TRUNC | O_WRONLY, 777);
	if(fd > 0){
		char *stringa = "ABCDEFGHILMNOPQRSTUVZ";
		int bytesWritten = write(fd,stringa,strlen(stringa));
		printf("Scritti %d bytes\n", bytesWritten );
	}

	close(fd);
	return 0;
}