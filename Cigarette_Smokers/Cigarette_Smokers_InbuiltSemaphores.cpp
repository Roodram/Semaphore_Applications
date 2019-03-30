#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

using namespace std;

#define MAX_QUEUE_LEN 10

int waitingSmokers=0;

sem_t matchSmoker, paperSmoker, tobaccoSmoker, agent;
pthread_mutex_t mutex;

int maxSmokes = 15;
int smokes = 0;
void* agentFunc(void* arg){
    int produced=0;
    while(produced<maxSmokes){
        
        pthread_mutex_lock(&mutex);
        int random = rand()%3;
       
        produced++;
        if(random==1){
            printf("Tobacco and paper on table %d\n", produced);
            sem_post(&matchSmoker);
        }
        else if(random == 2){
            printf("Tobacco and match on table %d\n", produced);
            sem_post(&paperSmoker);
        }
        else{
            printf("Match and paper on table %d\n", produced);
            sem_post(&tobaccoSmoker);
        }
        
        pthread_mutex_unlock(&mutex);
        sem_wait(&agent);

    }
}

void* tobaccoSmokerFunc(void* arg){
    while(smokes<maxSmokes){    
        sem_wait(&tobaccoSmoker);
        pthread_mutex_lock(&mutex);
        
        smokes++;
        printf("Tobacco Smoker picks up Match and Paper\n");
        sem_post(&agent);
        pthread_mutex_unlock(&mutex);
        printf("Tobacco smoker smokes %d\n\n", smokes);
        
        if(smokes>=maxSmokes){
            exit(0);
        }

    }
    
}

void* matchSmokerFunc(void* arg){
    while(smokes<maxSmokes){
        sem_wait(&matchSmoker);
        pthread_mutex_lock(&mutex);
        
        smokes++;
        printf("Match Smoker picks up Tobacco and Paper\n");
        sem_post(&agent);
        pthread_mutex_unlock(&mutex);
        printf("Match smoker smokes %d\n\n", smokes);
        
        if(smokes>=maxSmokes){
            exit(0);
        }
    }
}

void* paperSmokerFunc(void* arg){
    while(smokes<maxSmokes){
        sem_wait(&paperSmoker);
        pthread_mutex_lock(&mutex);
        
        smokes++;
        printf("Paper Smoker picks up Match and Tobacco\n");
        sem_post(&agent);        
        pthread_mutex_unlock(&mutex);
        printf("Paper smoker smokes %d\n\n", smokes);
        if(smokes>=maxSmokes){
            exit(0);
        }
    }    
}

int main(){
    pthread_t paper, tobacco, match, agentThread;

    sem_init(&paperSmoker, 0, 0);
    sem_init(&matchSmoker, 0, 0);
    sem_init(&tobaccoSmoker, 0, 0);
    sem_init(&agent, 0, 0);

    pthread_mutex_init(&mutex, NULL);
    
    
    pthread_create(&agentThread, NULL, agentFunc, NULL);
    
    pthread_create(&paper, NULL, paperSmokerFunc, NULL);
    pthread_create(&match, NULL, matchSmokerFunc, NULL);
    pthread_create(&tobacco, NULL, tobaccoSmokerFunc, NULL);

    pthread_exit(NULL);

}
