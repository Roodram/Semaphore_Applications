#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, wrt;

int readerCount=0;
int maxReads = 20;
int maxWrites = 20;
void *reader(void *number){
	int reads = 0;
	while(reads<maxReads){
				
		usleep(2);
		sem_wait(&mutex);
		
		readerCount++;

		if(readerCount==1){ 
			sem_wait(&wrt);
		}

		sem_post(&mutex);
		
		printf("Reader %s is reading\n", (char *)number);
		usleep(3);
		sem_wait(&mutex);
		
		readerCount--;

		if(readerCount==0){
			sem_post(&wrt);
		}
		reads++;
		sem_post(&mutex);
	}
}

void *writer(void *number){

	int writes = 0;
	while(writes<maxWrites){
		
		sem_wait(&wrt);
		printf("Writer %s is writing\n", (char *)number);
		sem_post(&wrt);
		usleep(2);
		writes++;
	}
	
}

int main(){
	pthread_t reader1, reader2, writer1, writer2, reader3;
	
	sem_init(&mutex,0,1);
	sem_init(&wrt,0,1);

	
	pthread_create(&reader1, NULL, reader, (void *)"1");
	pthread_create(&reader2, NULL, reader, (void *)"2");
	pthread_create(&writer1, NULL, writer, (void *)"1");
	pthread_create(&writer2, NULL, writer, (void *)"2");
	pthread_create(&reader3, NULL, reader, (void *)"3");

	pthread_exit(NULL);
	
}
