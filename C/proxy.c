
/* delegate the real work to proxy object */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Subject Subject_t;

struct Subject {
	void (*Request)(Subject_t *pThis);
};

Subject_t *Subject_Init(Subject_t *pThis) {
	return pThis;
}

typedef struct RealSubject {
	Subject_t sub;
} RealSubject_t;

void RealSubject_Request(RealSubject_t *pThis) {
	printf("RealSubject: Handling request.\n");
}

RealSubject_t *RealSubject_Init(RealSubject_t *pThis) {
	pThis->sub.Request = (void (*)(Subject_t *))RealSubject_Request; /* type casting */
}

typedef struct Proxy {
	Subject_t sub;
	
	RealSubject_t *real_subject_;
}Proxy_t;

bool Proxy_CheckAccess(Proxy_t *pThis) {
	printf("Proxy: Checking access prior to firing a real request.\n");
	return true;
}

void Proxy_LogAccess(Proxy_t *pThis) {
	printf("Proxy: Logging the time of request.\n");
}

void Proxy_Request(Proxy_t *pThis) {
	if(Proxy_CheckAccess(pThis)) {
		Subject_t *realSub = (Subject_t *)pThis->real_subject_; /* upcasting */
		realSub->Request(realSub);
		Proxy_LogAccess(pThis);
	}
}

/* ReadSubject instance */
RealSubject_t g_realSub;

Proxy_t *Proxy_Init(Proxy_t *pThis, RealSubject_t *real_subject) {
	pThis->sub.Request = (void (*)(Subject_t *))Proxy_Request; /* type casting */
	
	/* instead of copy constructor : But meaningless here */
	RealSubject_Init(&g_realSub);
	g_realSub = *real_subject;	
	pThis->real_subject_ = &g_realSub;
}

void ClientCode(Subject_t *subject) {
  // ...
  subject->Request(subject);
  // ...
}

int main() {
  printf("Client: Executing the client code with a real subject:\n");
  RealSubject_t real_subject;
  RealSubject_Init(&real_subject);  
  ClientCode((Subject_t *)&real_subject);  /* upcasting */
  printf("\n");
  
  printf("Client: Executing the same client code with a proxy:\n");  
  Proxy_t proxy;
  Proxy_Init(&proxy, &real_subject);  
  ClientCode((Subject_t *)&proxy);	  /* upcasting */

  return 0;
}
