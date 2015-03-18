/*Scrivere un programma in C e Posix sotto Linux che, preso un argomento intero positivo da riga di
 comando, gestisca la seguente situazione:
 genera due figli A e B e
 - se l'argomento è PARI invia un segnale SIGUSR1 alla ricezione del quale il figlio A calcola il cubo
 del numero passato come argomento da linea di comando, mentre il figlio B stampa un messaggio
 di arrivederci e termina.
 - se l'argomento è DISPARI invia un segnale SIGUSR2 alla ricezione del quale il figlio B calcola il
 reciproco del numero passato come argomento, attende per un numero di secondi pari al doppio
 del numero passato come argomento ed invia un segnale SIGUSR1 al processo A dopodiché
 termina l'esecuzione. Il figlio A, invece, attende la ricezione del segnale SIGUSR1, stampa un
 messaggio e termina.*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int numero;
pid_t pidA, pidB;

void handler1 (int);
void handler2 (int);
void handler3 (int);
int main (int argc, char *argv[]) {
    numero=atoi(argv[1]);
    
    if ((numero%2)==0) {  //pari
        pidA=fork();
        if (pidA < 0) {
            perror("Errore fork\n");
            exit(-1);
        }
        else if (pidA > 0) { //padre
            printf("Sono il padre con PID %d e invio SIGUSR1 al figlio A\n", getpid());
            kill(pidA, SIGUSR1);
            waitpid(pidA, NULL, 0); //aspetta che A finisca prima di creare B
            pidB=fork();
            if (pidB < 0) {
                perror("Errore fork\n");
                exit(-1);
            }
            else if(pidB>0) {
                waitpid(pidB, NULL, 0); //aspetta che B finisca per poi uscire
                printf("Sono il padre (pid=%d) e termino\n", getpid());
                exit(0);
            }
            else { //figlio B
                printf("Sono il figlio B (pid=%d) e termino\n", getpid());
                exit(1);
            }
        }
        else { //figlo A
            signal(SIGUSR1, handler1);
            pause();
            printf("Sono il processo A (PID=%d) e ora termino\n",getpid());
            exit(2);
        }
    }
    else {  //dispari
        pidA=fork();
        if (pidA < 0) {
            perror("Errore fork\n");
            exit(-1);
        }
        else if(pidA>0) { //padre
            pidB=fork();
            
            if (pidB < 0) {
                perror("Errore fork\n");
                exit(-1);
            }
            else if (pidB > 0) { //padre
                printf("Sono il padre (pid=%d) e invio SIGUSR2  a B\n", getpid());
                kill(pidB,SIGUSR2);
                waitpid(pidB, NULL, 0); //il padre prima di terminare aspetta che entrambi i figlio terminino
                waitpid(pidA, NULL, 0);
                printf("Sono il padre e termino\n");
                exit(0);
            }
            else { //figlio B
                signal(SIGUSR2, handler2);
                pause();
                
            }
            
        }
        else { //figlio A
            //waitpid(pidB, NULL, 0);
            signal(SIGUSR1, handler3);
            pause();
            
        }
    }
    
}

void handler1 (int signo)
{
    int cubo;
    if(signo==SIGUSR1) {
        cubo=numero*numero*numero;
        printf("Sono il figlio A (pid=%d), il cubo del numero e' %d\n", getpid(), cubo);
        exit(0);
    }
}

void handler2 (int signo) {
    float reciproco;
    int i;
    if(signo==SIGUSR2) {
        reciproco = (float)1/numero;
        printf("Sono il figlio B (pid=%d), il reciproco e' %f\n", getpid(), reciproco);
        printf("Sono B, aspetto %d secondi prima di inviare SIGUSR1 ad A e terminare\n", 2*numero); 
        for(i=1; i<=2*numero; i++) {
            printf("%d\n", i);
            sleep(1);
        }
        kill(pidA, SIGUSR1);
        exit(0);
    }
}

void handler3 (int signo) {
    if(signo==SIGUSR1) {
        printf("Sono il figlio A (pid=%d) e adesso termino\n", getpid());
        exit(0);
    }
}











