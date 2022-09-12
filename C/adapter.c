/* Wrapper between two objects */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

typedef struct Target Target_t;

struct Target {
	uint8_t *(* Request)(Target_t *);
};

uint8_t *Target_Request(Target_t *pThis) {
	return "Target: The default target's behavior.";
}

Target_t *Target_Init(Target_t *pThis) {
	pThis->Request = Target_Request;
}

typedef struct Adaptee {
	uint8_t anonymous;	/* empty variable */
}Adaptee_t;

uint8_t *Adaptee_SpecificRequest(Adaptee_t *pThis) {
	return ".eetpadA eht fo roivaheb laicepS";
}

Adaptee_t *Adaptee_Init(Adaptee_t *pThis) {
	pThis->anonymous = 0;
	return pThis;
}

typedef struct Adapter {
	Target_t target;
	
	Adaptee_t *adaptee_;
}Adapter_t;

uint8_t *Adapter_Request(Adapter_t *pThis) {
	uint8_t *to_reverse = Adaptee_SpecificRequest(pThis->adaptee_);
		
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t reverse[256];
	strcpy(reverse, to_reverse);	/* returned string is constant */
	strrev(reverse);
	static uint8_t buf[256];
	sprintf(buf, "%s%s", "Adapter: (TRANSLATED) ", reverse);
	return buf;
}

Adapter_t *Adapter_Init(Adapter_t *pThis, Adaptee_t *adaptee) {
	pThis->target.Request = (uint8_t *(*)(Target_t *))Adapter_Request; /* type casting */
	pThis->adaptee_ = adaptee;
	return pThis;
}

void ClientCode(Target_t *target) { /* removed const qualifier */
  printf("%s", target->Request(target));
}

int main() {
  printf("Client: I can work just fine with the Target objects:\n");
  Target_t target;
  Target_Init(&target);
  ClientCode(&target);
  printf("\n\n");
  
  Adaptee_t adaptee;
  Adaptee_Init(&adaptee);
  printf("Client: The Adaptee class has a weird interface. See, I don't understand it:\n");
  printf("Adaptee: %s", Adaptee_SpecificRequest(&adaptee));
  printf("\n\n");
  
  printf("Client: But I can work with it via the Adapter:\n");
  Adapter_t adapter;
  Adapter_Init(&adapter, &adaptee);
  ClientCode((Target_t *)&adapter);
  printf("\n");

  return 0;
}
