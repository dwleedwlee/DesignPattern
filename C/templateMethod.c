/* skeleton of an algorithm, override the steps  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * The Abstract Class defines a template method that contains a skeleton of some
 * algorithm, composed of calls to (usually) abstract primitive operations.
 *
 * Concrete subclasses should implement these operations, but leave the template
 * method itself intact.
 */
typedef struct AbstractClass AbstractClass_t;

struct AbstractClass {
	void (*RequiredOperations1)(AbstractClass_t *pThis);
	void (*RequiredOperation2)(AbstractClass_t *pThis);
	
	void (*Hook1)(AbstractClass_t *pThis);
	void (*Hook2)(AbstractClass_t *pThis);
};

void AbstractClass_Hook1(AbstractClass_t *pThis) {
}

void AbstractClass_Hook2(AbstractClass_t *pThis) {
}

void AbstractClass_BaseOperation1(AbstractClass_t *pThis) {
	printf("AbstractClass says: I am doing the bulk of the work\n");
}

void AbstractClass_BaseOperation2(AbstractClass_t *pThis) {
	printf("AbstractClass says: But I let subclasses override some operations\n");
}

void AbstractClass_BaseOperation3(AbstractClass_t *pThis) {
	printf("AbstractClass says: But I am doing the bulk of the work anyway\n");
}

void AbstractClass_TemplateMethod(AbstractClass_t *pThis) {
	AbstractClass_BaseOperation1(pThis);
	pThis->RequiredOperations1(pThis);
	AbstractClass_BaseOperation2(pThis);
	pThis->Hook1(pThis);
	pThis->RequiredOperation2(pThis);
	AbstractClass_BaseOperation3(pThis);
	pThis->Hook2(pThis);
}

AbstractClass_t *AbstractClass_Init(AbstractClass_t *pThis) {
	pThis->Hook1 = AbstractClass_Hook1;
	pThis->Hook2 = AbstractClass_Hook2;
	return pThis;
}

/**
 * Concrete classes have to implement all abstract operations of the base class.
 * They can also override some operations with a default implementation.
 */
 
typedef struct ConcreteClass1 {
	AbstractClass_t absClass;
} ConcreteClass1_t;

void ConcreteClass1_RequiredOperations1(ConcreteClass1_t *pThis) {
	printf("ConcreteClass1 says: Implemented Operation1\n");
}

void ConcreteClass1_RequiredOperation2(ConcreteClass1_t *pThis) {
	printf("ConcreteClass1 says: Implemented Operation2\n");
}

ConcreteClass1_t *ConcreteClass1_Init(ConcreteClass1_t *pThis) {
	AbstractClass_Init(&(pThis->absClass));	/* Call constructor of parent class */
	pThis->absClass.RequiredOperations1 = (void (*)(AbstractClass_t *))ConcreteClass1_RequiredOperations1;
	pThis->absClass.RequiredOperation2 = (void (*)(AbstractClass_t *))ConcreteClass1_RequiredOperation2;
	return pThis;
}

typedef struct ConcreteClass2 {
	AbstractClass_t absClass;
} ConcreteClass2_t;

void ConcreteClass2_RequiredOperations1(ConcreteClass2_t *pThis) {
	printf("ConcreteClass2 says: Implemented Operation1\n");
}

void ConcreteClass2_RequiredOperation2(ConcreteClass2_t *pThis) {
	printf("ConcreteClass2 says: Implemented Operation2\n");
}

void ConcreteClass2_Hook1(ConcreteClass2_t *pThis) {
	printf("ConcreteClass2 says: Overridden Hook1\n");
}

ConcreteClass2_t *ConcreteClass2_Init(ConcreteClass2_t *pThis) {
	AbstractClass_Init(&(pThis->absClass));	/* Call constructor of parent class */
	pThis->absClass.RequiredOperations1 = (void (*)(AbstractClass_t *))ConcreteClass2_RequiredOperations1;
	pThis->absClass.RequiredOperation2 = (void (*)(AbstractClass_t *))ConcreteClass2_RequiredOperation2;
	pThis->absClass.Hook1 = (void (*)(AbstractClass_t *))ConcreteClass2_Hook1;
	return pThis;
}

void ClientCode(AbstractClass_t *class_) {
  // ...
  AbstractClass_TemplateMethod(class_);
  // ...
}

int main() {
  printf("Same client code can work with different subclasses:\n");
  ConcreteClass1_t concreteClass1;
  ConcreteClass1_Init(&concreteClass1);
  ClientCode((AbstractClass_t *)&concreteClass1); /* type casting */
  printf("\n");
  printf("Same client code can work with different subclasses:\n");
  ConcreteClass2_t concreteClass2;
  ConcreteClass2_Init(&concreteClass2);
  ClientCode((AbstractClass_t *)&concreteClass2); /* type casting */
  return 0;
}
