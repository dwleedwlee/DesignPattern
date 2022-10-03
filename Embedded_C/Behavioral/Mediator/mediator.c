/* communicate indirectly, through a mediator object  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

typedef struct BaseComponent BaseComponent_t;

typedef struct Mediator Mediator_t;

struct Mediator {
	void (*Notify)(Mediator_t *pThis, BaseComponent_t *sender, uint8_t *event);
};

struct BaseComponent {
	Mediator_t *mediator_;
};	

void BaseComponent_set_mediator(BaseComponent_t *pThis, Mediator_t *mediator) {
	pThis->mediator_ = mediator;
}

BaseComponent_t *BaseComponent_Init(BaseComponent_t *pThis, Mediator_t *mediator) {
	pThis->mediator_ = mediator;
}

typedef struct Component1 {
	BaseComponent_t base;
}Component1_t;

void Component1_DoA(Component1_t *pThis) {
	printf("Component 1 does A.\n");
	Mediator_t *ptr = pThis->base.mediator_;
	ptr->Notify(ptr, (BaseComponent_t *)pThis, "A");	/* upcasting */
}

void Component1_DoB(Component1_t *pThis) {
	printf("Component 1 does B.\n");
	Mediator_t *ptr = pThis->base.mediator_;
	ptr->Notify(ptr, (BaseComponent_t *)pThis, "B");	/* upcasting */
}

Component1_t *Component1_Init(Component1_t *pThis) {
	return pThis;
}

typedef struct Component2 {
	BaseComponent_t base;
}Component2_t;

void Component2_DoC(Component2_t *pThis) {
	printf("Component 2 does C.\n");
	Mediator_t *ptr = pThis->base.mediator_;
	ptr->Notify(ptr, (BaseComponent_t *)pThis, "C");	/* upcasting */
}

void Component2_DoD(Component2_t *pThis) {
	printf("Component 2 does D.\n");
	Mediator_t *ptr = pThis->base.mediator_;
	ptr->Notify(ptr, (BaseComponent_t *)pThis, "D");	/* upcasting */
}

Component2_t *Component2_Init(Component2_t *pThis) {
	return pThis;
}

typedef struct ConcreteMediator {
	Mediator_t medi;
	
	Component1_t *component1_;
	Component2_t *component2_;
} ConcreteMediator_t;

void ConcreteMediator_Notify(ConcreteMediator_t *pThis, BaseComponent_t *sender, uint8_t *event) {
    if (strcmp(event, "A") == 0) {
      printf("Mediator reacts on A and triggers following operations:\n");
      Component2_DoC(pThis->component2_);
    }
    if (strcmp(event, "D") == 0) {
      printf("Mediator reacts on D and triggers following operations:\n");
      Component1_DoB(pThis->component1_);
      Component2_DoC(pThis->component2_);
    }	
}

ConcreteMediator_t *ConcreteMediator_Init(ConcreteMediator_t *pThis, Component1_t *c1, Component2_t *c2) {
	pThis->medi.Notify = (void(*)(Mediator_t *, BaseComponent_t *, uint8_t *))ConcreteMediator_Notify;
	
	pThis->component1_ = c1;
	pThis->component2_ = c2;
	
	BaseComponent_set_mediator((BaseComponent_t *)pThis->component1_, (Mediator_t *)pThis);	/* upcasting */
	BaseComponent_set_mediator((BaseComponent_t *)pThis->component2_, (Mediator_t *)pThis);	/* upcasting */
}

void ClientCode() {
  Component1_t c1;
  Component1_Init(&c1);
  Component2_t c2;
  Component2_Init(&c2);
  ConcreteMediator_t mediator;
  ConcreteMediator_Init(&mediator, &c1, &c2);

  printf("Client triggers operation A.\n");
  Component1_DoA(&c1);
  printf("\n");
  printf("Client triggers operation D.\n");
  Component2_DoD(&c2);
}

int main() {
  ClientCode();
  return 0;
}
