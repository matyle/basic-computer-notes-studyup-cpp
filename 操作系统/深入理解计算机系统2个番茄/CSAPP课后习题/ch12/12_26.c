// CSAPP 12.26
// 

#include <netdb.h>
#include <semaphore.h>
#include "csapp.h"

sem_t mutex;
Sem_init(&mutex, 0, 1);

// struct hostent {
// 	char	*h_name;
// 	char 	**h_aliases;
// 	int 	h_addrtype;
// 	int 	h_length;
// 	char	**h_addr_list;
// };

// struct hostent *gethostbyname(const char *name);


struct hostent *gethostbyname_ts(const char *name, struct hostent *private_hostp)
{
	struct hostent *shared_hostp;

	P(&mutex);		//加锁
	shared_hostp = gethostbyname(name);		//深层拷贝
	strcpy(private_hostp->h_name, shared_hostp->h_name);
	private_hostp->h_aliases = shared_hostp->h_aliases;
	private_hostp->h_addrtype = shared_hostp->h_addrtype;
	private_hostp->h_length = shared_hostp->h_length;
	private_hostp->h_addr_list = shared_hostp->h_addr_list;
	V(&mutex);

	return private_hostp;
}
