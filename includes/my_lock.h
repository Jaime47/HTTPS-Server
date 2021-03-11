#ifndef _my_lock
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>

#define _my_lock

void my_lock_init();

void my_lock_wait();

void my_lock_release();

#endif