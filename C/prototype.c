/*
  The MIT License (MIT)
  
  Copyright (c) 2022 Dongwook Lee (elight.lee@gmail.com)
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* create a clone of an object using copy constructor */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_PROTOTYPE_INSTANCES	(10)

typedef enum {
  PROTOTYPE_1 = 0,
  PROTOTYPE_2,
  PROTOTYPE_MAX
}Type_t;

typedef struct Prototype Prototype_t;

struct Prototype {
  uint8_t *prototype_name_;
  float prototype_field_;
  
  /* virtual functions */
  Prototype_t *(* Clone)(Prototype_t *pThis);
  void (*Method)(Prototype_t *pThis, float prototype_field);
};

void Prototype_Method(Prototype_t *pThis, float prototype_field) {
	pThis->prototype_field_ = prototype_field;
	printf("%s%s%s%f\n", "Call Method from ", pThis->prototype_name_, " with field : ", prototype_field);
}

/* non-virtual function */  
Prototype_t *Prototype_Init(Prototype_t *pThis, uint8_t *prototype_name) {
	pThis->Method = Prototype_Method;
	pThis->prototype_name_ = prototype_name;
	return pThis;
}

typedef struct ConcretePrototype1 {
	Prototype_t proto;
	
	float concrete_prototype_field1_;
}ConcretePrototype1_t;

/* a single instance : do not allow dynamic memory allocation in target embedded system */
/* it can be extended multiple instances */
ConcretePrototype1_t g_conProto1[NUM_PROTOTYPE_INSTANCES];

ConcretePrototype1_t *ConcretePrototype1_Clone(ConcretePrototype1_t *pThis) {
	g_conProto1[1] = *pThis;	/* using single instance */
	return &(g_conProto1[1]);
}

ConcretePrototype1_t *ConcretePrototype1_Init(ConcretePrototype1_t *pThis, uint8_t *prototype_name_, float concrete_prototype_field) {
	Prototype_Init(&(pThis->proto), prototype_name_);
	pThis->proto.Clone = (Prototype_t *(*)(Prototype_t *))ConcretePrototype1_Clone; /* type casting */
	pThis->concrete_prototype_field1_ = concrete_prototype_field;
	return pThis;
}

typedef struct ConcretePrototype2 {
	Prototype_t proto;
	
	float concrete_prototype_field2_;
}ConcretePrototype2_t;

/* a single instance : do not allow dynamic memory allocation in target embedded system */
/* it can be extended multiple instances */
ConcretePrototype2_t g_conProto2[NUM_PROTOTYPE_INSTANCES];

ConcretePrototype2_t *ConcretePrototype2_Clone(ConcretePrototype2_t *pThis) {
	g_conProto2[1] = *pThis;	/* using single instance */
	return &(g_conProto2[1]);
}

ConcretePrototype2_t *ConcretePrototype2_Init(ConcretePrototype2_t *pThis, uint8_t *prototype_name_, float concrete_prototype_field) {
	Prototype_Init(&(pThis->proto), prototype_name_);
	pThis->proto.Clone = (Prototype_t *(*)(Prototype_t *))ConcretePrototype2_Clone; /* type casting */
	pThis->concrete_prototype_field2_ = concrete_prototype_field;
	return pThis;
}

typedef struct PrototypeFactory {
	Prototype_t *prototypes_[PROTOTYPE_MAX]; /* unordered map -> set associative array */
}PrototypeFactory_t;

PrototypeFactory_t *PrototypeFactory_Init(PrototypeFactory_t *pThis) {
	pThis->prototypes_[PROTOTYPE_1] = (Prototype_t *)ConcretePrototype1_Init(&(g_conProto1[0]), "PROTOTYPE_1 ", 50.f);	/* upcasting */
	pThis->prototypes_[PROTOTYPE_2] = (Prototype_t *)ConcretePrototype2_Init(&(g_conProto2[0]), "PROTOTYPE_2 ", 60.f);	/* upcasting */
	return pThis;
}

Prototype_t *PrototypeFactory_CreatePrototype(PrototypeFactory_t *pThis, Type_t type) {
	return pThis->prototypes_[type]->Clone(pThis->prototypes_[type]);
}

void Client(PrototypeFactory_t *prototype_factory) {
  printf("Let's create a Prototype 1\n");

  Prototype_t *prototype = PrototypeFactory_CreatePrototype(prototype_factory, PROTOTYPE_1);
  prototype->Method(prototype, 90);

  printf("\n");

  printf("Let's create a Prototype 2 \n");

  prototype = PrototypeFactory_CreatePrototype(prototype_factory, PROTOTYPE_2);
  prototype->Method(prototype, 10);
}

int main() {
  PrototypeFactory_t prototype_factory;	/* object in the stack */
  PrototypeFactory_Init(&prototype_factory); /* init the object */
  Client(&prototype_factory);

  return 0;
}
