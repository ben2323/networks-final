/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#include "Semaphore.h"

Semaphore::Semaphore(int n)
{
	sem_init(&sem,0,n);
}

Semaphore::~Semaphore()
{
	sem_destroy(&sem);
}

void Semaphore::lock()
{
	sem_wait(&sem);
}

void Semaphore::unlock()
{
	sem_post(&sem);
}

