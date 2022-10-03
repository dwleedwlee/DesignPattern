
/* thread safe global variable */ 

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>	/* POSIX sleep in seconds */

typedef struct Singleton Singleton_t;

struct Singleton {
	Singleton_t *pinstance;
	pthread_mutex_t mutex_;
	
	uint8_t *value_;
};

static Singleton_t g_singleton;

/* Protected constructor : static function can not be accessed from outside of the file */
static Singleton_t *Singleton_Init(Singleton_t *pThis, const uint8_t *value) {
	pThis->value_ = (uint8_t *)value;
	return pThis;
}

/* Does not support constructor called in startup code - need to be called in main function */
void Singleton_ResourceInit(void) {
	pthread_mutex_init(&(g_singleton.mutex_), NULL);
}

/* static member function does not have this pointer as a parameter */
Singleton_t *Singleton_GetInstance(uint8_t *value) {	
	pthread_mutex_lock(&(g_singleton.mutex_)); /* mutex lock - thread safe */
	if(g_singleton.pinstance == NULL) {
		g_singleton.pinstance = Singleton_Init(&g_singleton, value); /* pointing itself */
	}
	pthread_mutex_unlock(&(g_singleton.mutex_));
	
	return g_singleton.pinstance;
}

uint8_t *Singleton_value(Singleton_t *pThis) {
	return pThis->value_;
}

void *ThreadFoo(void *arg){
    // Following code emulates slow initialization.
    sleep(1);
    Singleton_t* singleton = Singleton_GetInstance("FOO");
    printf("%s\n", Singleton_value(singleton));
}

void *ThreadBar(void *arg){
    // Following code emulates slow initialization.
    sleep(1);
    Singleton_t* singleton = Singleton_GetInstance("BAR");
    printf("%s\n", Singleton_value(singleton));
}

int main()
{   
    printf("If you see the same value, then singleton was reused (yay!)\n"
                "If you see different values, then 2 singletons were created (booo!!)\n\n"
                "RESULT:\n");
	pthread_t t1, t2;
	void *thread_status1, *thread_status2;
	
	Singleton_ResourceInit();
	
	pthread_create(&t1, NULL, ThreadFoo, NULL);
	pthread_create(&t2, NULL, ThreadBar, NULL);
	pthread_join(t1, &thread_status1);
	pthread_join(t2, &thread_status2);
    
    return 0;
}
