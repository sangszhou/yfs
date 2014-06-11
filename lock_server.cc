// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
	pthread_mutex_init(&m_,0);
	pthread_cond_init(&c_, 0);
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r)
{
	pthread_mutex_lock(&m_);
	lock_protocol::status ret = lock_protocol::OK;

	// require a new lock and assign to client directly
	if(lock_status_.find(lid) == lock_status_.end()) { 
		lock_status_[lid] = LOCKED;
		pthread_mutex_unlock(&m_);
		return ret;
	} 
	// if an old lock is acquired wait for this lock
	while(lock_status_[lid] == LOCKED) {
		pthread_cond_wait(&c_, &m_);
	}
	// obtain this lock
	lock_status_[lid] = LOCKED; 
	pthread_mutex_unlock(&m_);
	return ret;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
	pthread_mutex_lock(&m_);
	lock_protocol::status ret = lock_protocol::OK;
	// check malicious release
	if(lock_status_.find(lid) == lock_status_.end() ||
		lock_status_[lid] == FREE) {
		pthread_mutex_unlock(&m_);
		return ret;
	}
	lock_status_[lid] = FREE;
	pthread_cond_broadcast(&c_);
	pthread_mutex_unlock(&m_);
	return ret;
}
