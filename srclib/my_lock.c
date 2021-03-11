
#include "my_lock.h"

static pthread_mutex_t	*mptr;


void my_lock_init(char *pathname)
{
	int	fd;
	pthread_mutexattr_t	mattr;

	fd = open("/dev/zero", O_RDWR, 0);

	mptr = Mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
				MAP_SHARED, fd, 0);
	close(fd);

	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(mptr, &mattr);
}

void my_lock_wait()
{
	pthread_mutex_lock(mptr);
}

void my_lock_release()
{
	pthread_mutex_unlock(mptr);
}