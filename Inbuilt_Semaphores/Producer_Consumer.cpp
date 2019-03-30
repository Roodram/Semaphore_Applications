#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int index=0;

sem_t bufferFull,bufferEmpty;
pthread_mutex_t mutex;

void* produceItem(void* arg){
	while(1){
		sleep(1);
		int item = rand()%100;
		sem_wait(&bufferEmpty);
		pthread_mutex_lock(&mutex);
		buffer[index++] = item;
		cout<<"Produced Item = "<<item<<endl;
		// usleep(2000);
		pthread_mutex_unlock(&mutex);
		sem_post(&bufferFull);
	}
}

void* consumeItem(void* arg){
	while(1){
		sleep(2);
		sem_wait(&bufferFull);
		pthread_mutex_lock(&mutex);
		int item = buffer[--index];
		cout<<"Consumed Item = "<<item<<endl;
		pthread_mutex_unlock(&mutex);
		sem_post(&bufferEmpty);
	}
}

int main(){
	pthread_t producer,consumer;

	sem_init(&bufferEmpty,0, BUFFER_SIZE);
	sem_init(&bufferFull,0,0);
	pthread_mutex_init(&mutex,NULL);
	
	pthread_create(&producer, NULL, produceItem, NULL);
	pthread_create(&consumer, NULL, consumeItem, NULL);
	pthread_exit(NULL);
}