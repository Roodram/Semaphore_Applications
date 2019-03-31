#ifndef semaphore2_H
#define semaphore2_H

#include<stdio.h>
#include<pthread.h>

struct semaphore{
	int sem_val;
	pthread_mutex_t mutex;
	pthread_cond_t condition;
};

semaphore s;

void semaphore_wait(semaphore *s)
{
	pthread_mutex_lock(&(s->mutex));
	while (s->sem_val == 0) {
		pthread_cond_wait(&(s->condition), &(s->mutex));
	}
	s->sem_val--;
	pthread_mutex_unlock(&(s->mutex));
}

void semaphore_signal(semaphore *s)
	{
	pthread_mutex_lock(&(s->mutex));
	s->sem_val++;
	pthread_cond_broadcast(&(s->condition));
	pthread_mutex_unlock(&(s->mutex));
}

void semaphore_init(semaphore *s, int val)
{
	s->sem_val = val;
}

#endif
