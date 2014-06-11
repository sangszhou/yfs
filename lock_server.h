// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"
class lock_server {

 protected:
  int nacquire;

typedef enum {
	FREE,
	LOCKED,
} lock_status_t;

private:
	std::map<lock_protocol::lockid_t, lock_status_t> lock_status_;
	pthread_cond_t c_;
	pthread_mutex_t m_;

 public:
  lock_server();
  ~lock_server() {
  	VERIFY(pthread_mutex_destroy(&m_) == 0);
  };
  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
  // l1 p1
  lock_protocol::status acquire(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status release(int clt, lock_protocol::lockid_t lid, int &);
  // l1 p1
};

#endif 







