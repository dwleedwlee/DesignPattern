/* separate hierarchies, delegate implementations, dependency injection - similar with abstract factory pattern */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Implementation Implementation_t;

struct Implementation {
	uint8_t *(* OperationImplementation)(Implementation_t *); /* pure virtual function */
};

typedef struct ConcreteImplementationA {
	Implementation_t impl;
}ConcreteImplementationA_t;

uint8_t *ConcreteImplementationA_OperationImplementation(ConcreteImplementationA_t *pThis) {
	return "ConcreteImplementationA: Here's the result on the platform A.\n";
}

ConcreteImplementationA_t *ConcreteImplementationA_Init(ConcreteImplementationA_t *pThis) {
	/* type casting */
	pThis->impl.OperationImplementation = (uint8_t *(*)(Implementation_t *))ConcreteImplementationA_OperationImplementation;
}

typedef struct ConcreteImplementationB {
	Implementation_t impl;
}ConcreteImplementationB_t;

uint8_t *ConcreteImplementationB_OperationImplementation(ConcreteImplementationB_t *pThis) {
	return "ConcreteImplementationB: Here's the result on the platform B.\n";
}

ConcreteImplementationB_t *ConcreteImplementationB_Init(ConcreteImplementationB_t *pThis) {
	/* type casting */
	pThis->impl.OperationImplementation = (uint8_t *(*)(Implementation_t *))ConcreteImplementationB_OperationImplementation;
}

typedef struct Abstraction Abstraction_t;

struct Abstraction {
	Implementation_t *implementation_;
	uint8_t *(* Operation)(Abstraction_t *);
};

uint8_t *Abstraction_Operation(Abstraction_t *pThis) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s", "Abstraction: Base operation with:\n", 
		pThis->implementation_->OperationImplementation(pThis->implementation_));
	return buf;
}

Abstraction_t *Abstraction_Init(Abstraction_t *pThis, Implementation_t *implementation) {
	pThis->implementation_ = implementation;
	pThis->Operation = Abstraction_Operation;	/* FIXME : always called whenever a subclass is instantiated */
	return pThis;
}

typedef struct ExtendedAbstraction {
	Abstraction_t abs;
}ExtendedAbstraction_t;

uint8_t *ExtendedAbstraction_Operation(ExtendedAbstraction_t *pThis) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s", "ExtendedAbstraction: Extended operation with:\n", 
		pThis->abs.implementation_->OperationImplementation(pThis->abs.implementation_));
	return buf;
}

ExtendedAbstraction_t *ExtendedAbstraction_Init(ExtendedAbstraction_t *pThis, Implementation_t *implementation) {
	Abstraction_Init(&(pThis->abs), implementation);
	pThis->abs.Operation = (uint8_t *(*)(Abstraction_t *))ExtendedAbstraction_Operation; /* overwritten */
	return pThis;
}

void ClientCode(Abstraction_t* abstraction) {
  // ...
  printf("%s", abstraction->Operation(abstraction));
  // ...
}

int main() {  
  ConcreteImplementationA_t conImplA;
  ConcreteImplementationA_Init(&conImplA);
  /* Liskov Subsitution Principle */
  Implementation_t *implementation = (Implementation_t *)&conImplA; /* upcasting */
  
  Abstraction_t abstract;
  Abstraction_Init(&abstract, implementation);
  /* Liskov Subsitution Principle */
  Abstraction_t *abstraction = &abstract;

  ClientCode(abstraction);
  printf("\n");
  
  ConcreteImplementationB_t conImplB;
  ConcreteImplementationB_Init(&conImplB);
  implementation = (Implementation_t *)&conImplB; /* upcasting */
  
  ExtendedAbstraction_t extAbstract;
  ExtendedAbstraction_Init(&extAbstract, implementation);
  abstraction = (Abstraction_t *)&extAbstract; /* upcasting */
  
  ClientCode(abstraction);

  return 0;
}
