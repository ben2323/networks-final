/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#include <semaphore.h>

class Semaphore {

private:
	sem_t sem;

public:
	Semaphore(int n);
	void lock();
	void unlock();
	~Semaphore();
};

#endif /* SEMAPHORE_H_ */
