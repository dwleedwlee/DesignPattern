
/* provides a simplified interface */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Subsystem1 {
	uint8_t anonymous;	/* empty variable */
} Subsystem1_t;

uint8_t *Subsystem1_Operation1(Subsystem1_t *pThis) {
	return "Subsystem1: Ready!\n";
}

uint8_t *Subsystem1_OperationN(Subsystem1_t *pThis) {
	return "Subsystem1: Go!\n";
}

Subsystem1_t *Subsystem1_Init(Subsystem1_t *pThis) {
	return pThis;
}

typedef struct Subsystem2 {
	uint8_t anonymous;	/* empty variable */
} Subsystem2_t;

uint8_t *Subsystem2_Operation1(Subsystem2_t *pThis) {
	return "Subsystem2: Get Ready!\n";
}

uint8_t *Subsystem2_OperationZ(Subsystem2_t *pThis) {
	return "Subsystem1: Fire!\n";
}

Subsystem2_t *Subsystem2_Init(Subsystem2_t *pThis) {
	return pThis;
}

typedef struct Facade {
	Subsystem1_t *subsystem1_;
	Subsystem2_t *subsystem2_;
} Facade_t;

uint8_t *Facade_Operation(Facade_t *pThis) {	
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s%s%s%s%s", 
	"Facade initializes subsystems:\n", 
	Subsystem1_Operation1(pThis->subsystem1_),
	Subsystem2_Operation1(pThis->subsystem2_),
	"Facade orders subsystems to perform the action:\n",
	Subsystem1_OperationN(pThis->subsystem1_),
	Subsystem2_OperationZ(pThis->subsystem2_));	
	return buf;
}

/* Actual subsystem objects */
Subsystem1_t g_subsystem1;
Subsystem2_t g_subsystem2;

Facade_t *Facade_Init(Facade_t *pThis, Subsystem1_t *subsystem1, Subsystem2_t *subsystem2) {
	if(subsystem1) {
		pThis->subsystem1_ = subsystem1;
	} else {
		Subsystem1_Init(&g_subsystem1);
		pThis->subsystem1_ = &g_subsystem1;
	}
	if(subsystem2) {
		pThis->subsystem2_ = subsystem2;
	} else {
		Subsystem2_Init(&g_subsystem2);
		pThis->subsystem2_ = &g_subsystem2;
	}	
}

void ClientCode(Facade_t *facade) {
  // ...
  printf("%s", Facade_Operation(facade));
  // ...
}

int main() {
  Subsystem1_t subsystem1;
  Subsystem1_Init(&subsystem1);
  Subsystem2_t subsystem2;
  Subsystem2_Init(&subsystem2);
  Facade_t facade;
  Facade_Init(&facade, &subsystem1, &subsystem2);
  //Facade_Init(&facade, NULL, NULL);
  ClientCode(&facade);

  return 0;
}
