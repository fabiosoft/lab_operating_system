//
//  mutex1.c
//  
//
//  Created by Fabio Nisci on 18/06/14.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * functionC();
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main(int argc, char *argv[]){
    int rc1, rc2;
    pthread_t thread1, thread2;
    
    if((rc1=pthread_create(&thread1, NULL, &functionC,NULL))){
        printf("thread1 creation failed");
    }
    
    
    if((rc2=pthread_create(&thread2, NULL, &functionC,NULL))){
        printf("thread2 creation failed");
    }
    
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    exit(EXIT_SUCCESS);
}

void * functionC(){
    pthread_mutex_lock(&mutex1);
    counter++;
    printf("Counter value: %d\n",counter);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}
