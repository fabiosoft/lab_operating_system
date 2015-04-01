//
//  mutex_join.c
//  
//
//  Created by Fabio Nisci on 18/06/14.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 10
void * thread_function();
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main(int argc, char *argv[]){
    pthread_t thread_id[NTHREADS];
    int i, j;
    
    for(i = 0; i < NTHREADS; i++){
        pthread_create(&thread_id[i], NULL, thread_function,NULL);
    }
    
    for(j = 0; j < NTHREADS; j++){
        pthread_join(thread_id[j], NULL);
    }
    
    printf("Final counter value: %d\n",counter);
    
    exit(EXIT_SUCCESS);
}

void * thread_function(){
    printf("Thread number: %ld\n",(long)pthread_self());
    pthread_mutex_lock(&mutex1);
    counter++;
    pthread_mutex_unlock(&mutex1);
    return NULL;
}
