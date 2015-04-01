#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

int pari = 0;

void scambiaPari(){
	pari = 1; // 1 = dispari | 0 = pari
}

int main(int argc, char *argv[]){
	

	pid_t p11, p12;
	int pa[2], pb[2];

	pipe(pa);
	pipe(pb);

	int N = 5;

	if((p11 = fork())){
	    if((p12 = fork())){
	    	//padre P1
	    	//sleep(1);
	    	printf("PIDS: %d %d\n",p11,p12);

	    	close(pa[0]);
	    	for(int i = 0; i < N; i++){
	    		char *scrivo = malloc(1 * sizeof(int));
	    		sprintf(scrivo,"%d",i+1);
	    		write(pa[1],scrivo,sizeof(scrivo));
	    	}


			
            waitpid(p11,NULL,0);
            waitpid(p12,NULL,0);
	    }else{
	    	//p12
	    	char *bufPari = malloc((N/2) * sizeof(int));
	    	//preleva pari
	    	for (int l = 1; l <= N/2; l++){
	    		char *letto = malloc(1 * sizeof(int));
				read(pb[0],letto,sizeof(letto));
				strcat(bufPari, letto);
	    	}
	    	//scrive su file
			unlink("primofile.txt");
			int primofile = open("primofile.txt",O_CREAT|O_TRUNC|O_WRONLY,777);
			write(primofile,bufPari,sizeof(bufPari));
			kill(p11,SIGUSR1);

	    	char *bufdispari = malloc((N/2) * sizeof(int));
	    	//preleva dispari
			for (int l = 0; l <= N/2; l++){
	    		char *letto = malloc(1 * sizeof(int));
				read(pb[0],letto,sizeof(letto));
				strcat(bufdispari, letto);
	    	}

	    	//scrive su file
			unlink("secondofile.txt");
			int secondofile = open("secondofile.txt",O_CREAT|O_TRUNC|O_WRONLY,777);
			write(secondofile,bufdispari,sizeof(bufdispari));

	    	exit(0);
	    }
	}else{
		//p11
		signal(SIGUSR1,scambiaPari);
		close(pa[1]);
		close(pb[0]);
		//colleziona i numeri
		int *numeri = malloc(N * sizeof(int));
		for(int i = 0; i < N; i++){
			char *letto = malloc(1 * sizeof(int));
			read(pa[0],letto,sizeof(letto));
			numeri[i] = atoi(letto);
		
			if((atoi(letto) % 2) == pari){
				//pari
				write(pb[1],letto,sizeof(letto));
			}
		}
		pause();
		for(int i = 0; i < N; i++){
			if((numeri[i] % 2) == pari){
				//dispari
				char *invio = malloc(1 * sizeof(int));
				sprintf(invio,"%d",numeri[i]);
				write(pb[1],invio,sizeof(invio));
			}
		}


		exit(0);
	}

	return 0;
}