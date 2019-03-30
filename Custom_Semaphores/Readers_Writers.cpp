#include <stdio.h>
#include <pthread.h>
#include "semaphore2.h"
#include <unistd.h>

semaphore mutex, wrt;

int readerCount=0;
int maxReads = 20;
int maxWrites = 20;
void *reader(void *number){
	int reads = 0;
	while(reads<maxReads){
		
		usleep(2);
		semaphore_wait(&mutex);
		readerCount++;

		if(readerCount==1){ 
			semaphore_wait(&wrt);
		}

		semaphore_signal(&mutex);
		printf("Reader %s is reading\n", (char *)number);
		usleep(3);
		semaphore_wait(&mutex);
		readerCount--;

		if(readerCount==0){
			semaphore_signal(&wrt);
		}
		reads++;
		semaphore_signal(&mutex);
	}
}

void *writer(void *number){
	int writes = 0;
	while(writes<maxWrites){
		
		semaphore_wait(&wrt);
		printf("Writer %s is writing\n", (char *)number);
		semaphore_signal(&wrt);
		usleep(2);
		writes++;
	}
	
}

int main(){
	pthread_t reader1, reader2, writer1, writer2, reader3;
	
	semaphore_init(&mutex,1);
	semaphore_init(&wrt,1);

	
	pthread_create(&reader1, NULL, reader, (void *)"1");
	pthread_create(&reader2, NULL, reader, (void *)"2");
	pthread_create(&writer1, NULL, writer, (void *)"1");
	pthread_create(&writer2, NULL, writer, (void *)"2");
	pthread_create(&reader3, NULL, reader, (void *)"3");

	pthread_exit(NULL);
	
}