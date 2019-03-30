#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

using namespace std;

#define MAX_QUEUE_LEN 10

int waitingCustomers=0;
int maxCustomers = 40;
sem_t customerQueue, barberReady;
pthread_mutex_t mutex;

void* barber(void* arg){
	int haircuts = 0;
	while(haircuts<maxCustomers){
		
		sem_wait(&customerQueue);		
		pthread_mutex_lock(&mutex);

		
		waitingCustomers--;
		haircuts++;

		sem_post(&barberReady);
		pthread_mutex_unlock(&mutex);
		
		
		printf("Barber is cutting hair of customer %d \nCustomers in queue %d \n\n", haircuts, waitingCustomers);
		usleep(100000);
	}
}

void* customer(void* arg){
	int customers = 0;
	while(customers<maxCustomers){
		pthread_mutex_lock(&mutex);

		if (waitingCustomers <= MAX_QUEUE_LEN) {
   			
			waitingCustomers++;
			customers++;

			printf("Customer %d is in\n\n", customers);
			sem_post(&customerQueue);
			pthread_mutex_unlock(&mutex);
			sem_wait(&barberReady);

			// printf("Customer %d is in\n\n", customers);
		}
		else{
			pthread_mutex_unlock(&mutex);
		}
	}

}

int main(){
	pthread_t barberThread, customerThread;

	sem_init(&barberReady, 0, 0);
	sem_init(&customerQueue, 0, 0);

	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&barberThread, NULL, barber, NULL);
	
	pthread_create(&customerThread, NULL, customer, NULL);

	pthread_exit(NULL);
}



