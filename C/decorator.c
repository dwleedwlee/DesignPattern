
/* adding new behaviors, wrapper, decorators following same interface */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Component Component_t;

struct Component {
    uint8_t *(*Operation)(Component_t *pThis);
};

typedef struct ConcreteComponent {
	Component_t comp;
} ConcreteComponent_t;

uint8_t *ConcreteComponent_Operation(ConcreteComponent_t *pThis) {
	return "ConcreteComponent";
}

ConcreteComponent_t *ConcreteComponent_Init(ConcreteComponent_t *pThis) {
	pThis->comp.Operation = (uint8_t *(*)(Component_t *))ConcreteComponent_Operation; /* type casting */
}

typedef struct Decorator Decorator_t;

struct Decorator {
	Component_t comp;
	
	Component_t *component_;
};

uint8_t *Decorator_Operation(Decorator_t *pThis) {
	return pThis->component_->Operation(pThis->component_); /* pThis->component_ : owner object of Operation method */
}

Decorator_t *Decorator_Init(Decorator_t *pThis, Component_t *component) {
	pThis->component_ = component;
	pThis->comp.Operation = (uint8_t *(*)(Component_t *))Decorator_Operation; /* type casting */
	return pThis;
}

typedef struct ConcreteDecoratorA {
	Decorator_t deco;
} ConcreteDecoratorA_t;

uint8_t *ConcreteDecoratorA_Operation(ConcreteDecoratorA_t *pThis) {	
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s%s", "ConcreteDecoratorA(", Decorator_Operation((Decorator_t *)pThis), ")"); /* Decorator::Operation() */
	return buf;
}

ConcreteDecoratorA_t *ConcreteDecoratorA_Init(ConcreteDecoratorA_t *pThis, Component_t *component) {
	Decorator_Init((Decorator_t *)pThis, component); /* upcasting */
	pThis->deco.comp.Operation = (uint8_t *(*)(Component_t *))ConcreteDecoratorA_Operation; /* type casting */
	return pThis;
}

typedef struct ConcreteDecoratorB {
	Decorator_t deco;
} ConcreteDecoratorB_t;

uint8_t *ConcreteDecoratorB_Operation(ConcreteDecoratorB_t *pThis) {	
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s%s", "ConcreteDecoratorB(", Decorator_Operation((Decorator_t *)pThis), ")"); /* Decorator::Operation() */
	return buf;
}

ConcreteDecoratorB_t *ConcreteDecoratorB_Init(ConcreteDecoratorB_t *pThis, Component_t *component) {
	Decorator_Init((Decorator_t *)pThis, component); /* upcasting */
	pThis->deco.comp.Operation = (uint8_t *(*)(Component_t *))ConcreteDecoratorB_Operation; /* type casting */
	return pThis;
}

void ClientCode(Component_t *component) {
  // ...
  printf("RESULT: %s", component->Operation(component));
  // ...
}

int main() {
  /**
   * This way the client code can support both simple components...
   */
  ConcreteComponent_t conComp;
  ConcreteComponent_Init(&conComp);
  Component_t *simple = (Component_t *)&conComp;
  printf("Client: I've got a simple component:\n");
  ClientCode(simple);
  printf("\n\n");
  /**
   * ...as well as decorated ones.
   *
   * Note how decorators can wrap not only simple components but the other
   * decorators as well.
   */
  ConcreteDecoratorA_t conDecoA;
  ConcreteDecoratorA_Init(&conDecoA, simple);
  Component_t *decorator1 = (Component_t *)&conDecoA;
  ConcreteDecoratorB_t conDecoB;
  ConcreteDecoratorB_Init(&conDecoB, decorator1);
  Component_t *decorator2 = (Component_t *)&conDecoB;
  printf("Client: Now I've got a decorated component:\n");
  ClientCode(decorator2);
  printf("\n");

  return 0;
}
