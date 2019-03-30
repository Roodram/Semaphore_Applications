#include <iostream>
#include <pthread.h>
#include "semaphore2.h"
#include <random>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int index=0;

semaphore bufferFull,bufferEmpty;
pthread_mutex_t mutex;

int maxItems = 7;

void* produceItem(void* arg){
	int produced = 0;
	while(produced<maxItems){
		usleep(30000);
		int item = rand()%100;
		semaphore_wait(&bufferEmpty);
		pthread_mutex_lock(&mutex);
		buffer[index++] = item;
		produced++;
		cout<<"Produced Item = "<<item<<"\n";
		// usleep(2000);
		pthread_mutex_unlock(&mutex);
		semaphore_signal(&bufferFull);
	}
}

void* consumeItem(void* arg){
	int consumed = 0;
	while(consumed<maxItems){
		usleep(60000);
		semaphore_wait(&bufferFull);
		pthread_mutex_lock(&mutex);
		int item = buffer[--index];
		consumed++;
		cout<<"Consumed Item = "<<item<<"\n";
		pthread_mutex_unlock(&mutex);
		semaphore_signal(&bufferEmpty);
	}
}

int main(){
	pthread_t producer,consumer;

	semaphore_init(&bufferEmpty, BUFFER_SIZE);
	semaphore_init(&bufferFull, 0);
	pthread_mutex_init(&mutex,NULL);
	
	pthread_create(&producer, NULL, produceItem, NULL);
	pthread_create(&consumer, NULL, consumeItem, NULL);
	pthread_exit(NULL);
}