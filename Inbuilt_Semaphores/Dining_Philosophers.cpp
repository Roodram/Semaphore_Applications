#include <pthread.h> 
#include <semaphore.h> 
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
sem_t mutex; 
sem_t S[N]; 
  
void tryEat(int philosopher) { 
    if (state[philosopher] == HUNGRY && state[left(philosopher)] != EATING && state[right(philosopher)] != EATING) { 
        state[philosopher] = EATING;   
 
        printf("Philosopher %d is eating\n", philosopher + 1); 
        usleep(10000);
        sem_post(&S[philosopher]); 
    } 
} 
  
void fork_down(int philosopher) {

    sem_wait(&mutex);
    state[philosopher] = THINKING; 
    printf("Philosopher %d is thinking\n", philosopher + 1); 
    tryEat(left(philosopher)); 
    tryEat(right(philosopher));

    sem_post(&mutex); 
} 

void fork_up(int philosopher) { 
  
    sem_wait(&mutex);    
    state[philosopher] = HUNGRY;  
    printf("Philosopher %d is Hungry\n", philosopher + 1);  
    tryEat(philosopher);   
    sem_post(&mutex);    
    sem_wait(&S[philosopher]);
    usleep(5000); 
} 
   
void* philosopherFunc(void* num) { 
  
    while (1) {   
        int* i = (int *)num;  
        fork_up(*i);
        usleep(5000);   
        fork_down(*i); 
    } 
} 
  
int main() { 
  
    int i; 
    pthread_t philosopher_thread[N];  
    sem_init(&mutex, 0, 1); 
  
    for (i = 0; i < N; i++){  
        sem_init(&S[i], 0, 0);
    } 
  
    for (i = 0; i < N; i++) { 

        pthread_create(&philosopher_thread[i], NULL, philosopherFunc, &philosophers[i]); 
        printf("Philosopher %d is thinking\n", i + 1); 
    } 
  
    for (i = 0; i < N; i++)   
        pthread_join(philosopher_thread[i], NULL); 
}