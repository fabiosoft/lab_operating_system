/*
• Modificare l'esercizio 1 in modo che prenda
l'input dal file “testfile” e scriva l'output nel file
“outputfile”
• Utilizzare le funzioni per la duplicazione dei file
descriptor
• TUTTE LE READ SU STANDARD INPUT
• TUTTE LE WRITE SU STANDARD OUTPUT
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	int fd1,fd2;
	char *buf = malloc(sizeof(char));
	if ((fd1=open("testfile.txt",O_RDONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO))>0){
		close(0);
		int fd1Dup = dup(fd1);
		printf("Dup:%d - fd1:%d\n", fd1Dup,fd1);
     	write(fd1Dup, "ciao mondo", 10*sizeof(char));
     	close(fd1Dup);
	}	

	return 0;
}