//
//  es10.c
//  Created by Fabio Nisci on 08/05/14.
//
// Realizzare un programma in C e Posix sotto Linux che realizzi una struttura di processi ad albero ternario, tale che ogni processo si metta in attesa che i suoi figli terminino. Ogni figlio termina dopo aver atteso per un numero di secondi pari al livello dell'albero al quale si trova, allo scadere del quale stampa a schermo la stringa "Concluso! e comunica la genitore la sua terminazione.

//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){
    
    int n_livelli = 10;
    int lvl_corrente = 0;
    int pid = -1;
    int status = 0;
    while (lvl_corrente <= n_livelli) {
        pid = fork();
        lvl_corrente++;
        if(pid < 0){
            //errore fork
            printf("errore fork()\n");
            exit(1);
        }
        if(pid == 0){
            //figlio
            printf("aspetto %d sec\n",lvl_corrente);
            sleep(lvl_corrente);
            printf("%d concluso!\n",getpid());
            exit(0);
        }
        if(pid > 0){
            //padre
            waitpid(pid,&status,0);
            if (WIFEXITED(status)) {
                printf("figlio terminato\n");
            }
        }
    }
}
