//
//  cond1.c
//  
//
//  Created by Fabio Nisci on 20/06/14.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

void * functionCount1();
void * functionCount2();

int count = 0;

#define COUNT_DONE 10
#define COUNT_HALT1 3
#define COUNT_HALT2 6

int main(int argc, char *argv[]){
    
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, &functionCount1,NULL);
    pthread_create(&thread2, NULL, &functionCount2,NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Final count: %d\n",count);
    exit(EXIT_SUCCESS);
}

//scrive numeri 1-3 e da 8-10 permessi dalla seconda funzione
void * functionCount1(){
    for(;;){
        pthread_mutex_lock(&count_mutex);
        //wait finchè function2 opera la signal sulla variabile di condizione condition_var
        pthread_cond_wait(&condition_var, &count_mutex);
        count++;
        printf("Counter value functionCount1: %d\n",count);
        pthread_mutex_unlock(&count_mutex);
        if(count >= COUNT_DONE){
            return NULL;
        }
    }
}

//scrive numeri 4-7
void * functionCount2(){
    for(;;){
        pthread_mutex_lock(&count_mutex);
        if(count < COUNT_HALT1 || count > COUNT_HALT2){
            //signal per liberare il thread in attesa rilasciando il mutex
            //function1 adesso ha il permesso di modificare count
            pthread_cond_signal(&condition_var);
        }else{
            count++;
            printf("Counter value functionCount2: %d\n",count);
        }
        pthread_mutex_unlock(&count_mutex);
        if(count >= COUNT_DONE){
            return NULL;
        }
    }
}



