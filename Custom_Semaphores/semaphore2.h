#ifndef semaphore2_H
#define semaphore2_H

#include<stdio.h>
#include<pthread.h>

struct semaphore{
	int val;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

semaphore s;

void semaphore_wait(semaphore *s)
{
	pthread_mutex_lock(&(s->mutex));
	while (s->val == 0) {
		pthread_cond_wait(&(s->cond), &(s->mutex));
	}
	s->val--;
	pthread_mutex_unlock(&(s->mutex));
}

void semaphore_signal(semaphore *s)
	{
	pthread_mutex_lock(&(s->mutex));
	s->val++;
	pthread_cond_broadcast(&(s->cond));
	pthread_mutex_unlock(&(s->mutex));
}

void semaphore_init(semaphore *s, int val)
{
	s->val = val;
}

#endif