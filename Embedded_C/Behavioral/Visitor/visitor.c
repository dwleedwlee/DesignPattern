/* adding a new behaviors  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * The Visitor Interface declares a set of visiting methods that correspond to
 * component classes. The signature of a visiting method allows the visitor to
 * identify the exact class of the component that it's dealing with.
 */
/* But, the visitor should know the concrete class of the component class */ 
typedef struct ConcreteComponentA ConcreteComponentA_t;
typedef struct ConcreteComponentB ConcreteComponentB_t;

typedef struct Visitor Visitor_t;

/* removed const qualifier - it does not support constant function in C */
struct Visitor {
	void (*VisitConcreteComponentA)(Visitor_t *pThis, ConcreteComponentA_t *element);
	void (*VisitConcreteComponentB)(Visitor_t *pThis, ConcreteComponentB_t *element);
};

/**
 * The Component interface declares an `accept` method that should take the base
 * visitor interface as an argument.
 */
typedef struct Component Component_t;

/* removed const qualifier - it does not support constant function in C */
struct Component {
	void (*Accept)(Component_t *pThis, Visitor_t *visitor);
};

/**
 * Each Concrete Component must implement the `Accept` method in such a way that
 * it calls the visitor's method corresponding to the component's class.
 */
struct ConcreteComponentA {
	Component_t comp;
};

void ConcreteComponentA_Accept(ConcreteComponentA_t *pThis, Visitor_t *visitor) {
	visitor->VisitConcreteComponentA(visitor, pThis);
}

uint8_t *ConcreteComponentA_ExclusiveMethodOfConcreteComponentA(ConcreteComponentA_t *pThis) {
	return "A";
}

ConcreteComponentA_t *ConcreteComponentA_Init(ConcreteComponentA_t *pThis) {
	pThis->comp.Accept = (void (*)(Component_t *, Visitor_t *))ConcreteComponentA_Accept;
	return pThis;
}

struct ConcreteComponentB {
	Component_t comp;
};

void ConcreteComponentB_Accept(ConcreteComponentB_t *pThis, Visitor_t *visitor) {
	visitor->VisitConcreteComponentB(visitor, pThis);
}

uint8_t *ConcreteComponentB_ExclusiveMethodOfConcreteComponentB(ConcreteComponentB_t *pThis) {
	return "B";
}

ConcreteComponentB_t *ConcreteComponentB_Init(ConcreteComponentB_t *pThis) {
	pThis->comp.Accept = (void (*)(Component_t *, Visitor_t *))ConcreteComponentB_Accept;
	return pThis;
}

/**
 * Concrete Visitors implement several versions of the same algorithm, which can
 * work with all concrete component classes.
 *
 * You can experience the biggest benefit of the Visitor pattern when using it
 * with a complex object structure, such as a Composite tree. In this case, it
 * might be helpful to store some intermediate state of the algorithm while
 * executing visitor's methods over various objects of the structure.
 */
typedef struct ConcreteVisitor1 {
	Visitor_t visitor;
}ConcreteVisitor1_t;


void ConcreteVisitor1_VisitConcreteComponentA(ConcreteVisitor1_t *pThis, ConcreteComponentA_t *element) {
	printf("%s + ConcreteVisitor1\n", ConcreteComponentA_ExclusiveMethodOfConcreteComponentA(element));
}

void ConcreteVisitor1_VisitConcreteComponentB(ConcreteVisitor1_t *pThis, ConcreteComponentB_t *element) {
	printf("%s + ConcreteVisitor1\n", ConcreteComponentB_ExclusiveMethodOfConcreteComponentB(element));
}

ConcreteVisitor1_t *ConcreteVisitor1_Init(ConcreteVisitor1_t *pThis) {
	pThis->visitor.VisitConcreteComponentA = 
	(void (*)(Visitor_t *, ConcreteComponentA_t *))ConcreteVisitor1_VisitConcreteComponentA;
	pThis->visitor.VisitConcreteComponentB = 
	(void (*)(Visitor_t *, ConcreteComponentB_t *))ConcreteVisitor1_VisitConcreteComponentB;
	return pThis;
}

typedef struct ConcreteVisitor2 {
	Visitor_t visitor;
}ConcreteVisitor2_t;

void ConcreteVisitor2_VisitConcreteComponentA(ConcreteVisitor2_t *pThis, ConcreteComponentA_t *element) {
	printf("%s + ConcreteVisitor2\n", ConcreteComponentA_ExclusiveMethodOfConcreteComponentA(element));
}

void ConcreteVisitor2_VisitConcreteComponentB(ConcreteVisitor2_t *pThis, ConcreteComponentB_t *element) {
	printf("%s + ConcreteVisitor2\n", ConcreteComponentB_ExclusiveMethodOfConcreteComponentB(element));
}

ConcreteVisitor2_t *ConcreteVisitor2_Init(ConcreteVisitor2_t *pThis) {
	pThis->visitor.VisitConcreteComponentA = 
	(void (*)(Visitor_t *, ConcreteComponentA_t *))ConcreteVisitor2_VisitConcreteComponentA;
	pThis->visitor.VisitConcreteComponentB = 
	(void (*)(Visitor_t *, ConcreteComponentB_t *))ConcreteVisitor2_VisitConcreteComponentB;
	return pThis;
}

void ClientCode(Component_t *components[], size_t n, Visitor_t *visitor) {
  // ...
  size_t i = 0;
  for (; i < n; i++) {
	components[i]->Accept(components[i], visitor);
  }
  // ...
}

#define NUM_MAX_COMPONENT_INDEX	(2)

int main() {
  ConcreteComponentA_t conCompA;
  ConcreteComponentA_Init(&conCompA);
  ConcreteComponentB_t conCompB;
  ConcreteComponentB_Init(&conCompB);
  Component_t *components[NUM_MAX_COMPONENT_INDEX] = {(Component_t *)&conCompA, (Component_t *)&conCompB}; /* type casting */

  printf("The client code works with all visitors via the base Visitor interface:\n");
  ConcreteVisitor1_t visitor1;
  ConcreteVisitor1_Init(&visitor1);
  ClientCode(components, NUM_MAX_COMPONENT_INDEX, (Visitor_t *)&visitor1);	/* type casting */
  printf("\n");
  printf("It allows the same client code to work with different types of visitors:\n");
  ConcreteVisitor2_t visitor2;
  ConcreteVisitor2_Init(&visitor2);
  ClientCode(components, NUM_MAX_COMPONENT_INDEX, (Visitor_t *)&visitor2);	/* type casting */

  return 0;
}
