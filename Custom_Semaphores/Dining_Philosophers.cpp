#include <pthread.h> 
#include "semaphore2.h"
#include <stdio.h> 
#include <unistd.h>

#define N 5
int THINKING = 2;
int HUNGRY = 1;
int EATING = 0;

int left(int philosopher){
    return (philosopher + 4)%N;
}

int right( int philosopher){
    return (philosopher+1)%N;
}

int state [N];  
int philosophers[] = {0, 1, 2, 3, 4}; 
semaphore mutex; 
semaphore S [N]; 
  
void tryEat(int philosopher) { 
    if (state[philosopher] == HUNGRY && state[left(philosopher)] != EATING && state[right(philosopher)] != EATING) { 
        state[philosopher] = EATING;   
 
        printf("Philosopher %d is eating\n", philosopher + 1); 
        usleep(10000);
        semaphore_signal(&S[philosopher]); 
    } 
} 
  
void fork_down(int philosopher) {

    semaphore_wait(&mutex);
    state[philosopher] = THINKING; 
    printf("Philosopher %d is thinking\n", philosopher + 1); 
    tryEat(left(philosopher)); 
    tryEat(right(philosopher));

    semaphore_signal(&mutex); 
} 

void fork_up(int philosopher) { 
  
    semaphore_wait(&mutex);    
    state[philosopher] = HUNGRY;  
    printf("Philosopher %d is Hungry\n", philosopher + 1);  
    tryEat(philosopher);   
    semaphore_signal(&mutex);    
    semaphore_wait(&S[philosopher]);
    usleep(5000); 
} 
   
void* philosopherFunc(void* num) { 
  
    while (1) {   
        int* i = (int *)num;  
        fork_up(*i);
        usleep(50);   
        fork_down(*i); 
    } 
} 
  
int main() { 
  
    int i; 
    pthread_t philosopher_thread[N];  
    semaphore_init(&mutex,  1); 
  
    for (i = 0; i < N; i++){  
        semaphore_init(&S[i], 0);
    } 
  
    for (i = 0; i < N; i++) { 

        pthread_create(&philosopher_thread[i], NULL, philosopherFunc, &philosophers[i]); 
        printf("Philosopher %d is thinking\n", i + 1); 
    } 
  
    for (i = 0; i < N; i++)   
        pthread_join(philosopher_thread[i], NULL); 
}