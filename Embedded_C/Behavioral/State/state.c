/* extracts state related behaviors, instead of switch-case statement  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * The base State class declares methods that all Concrete State should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */
 
typedef struct Context Context_t;

typedef struct State State_t;

struct State {
	Context_t *context_;
	
	void (*Handle1)(State_t *pThis);
	void (*Handle2)(State_t *pThis);
	const uint8_t * (*name)(State_t *pThis); /* instead of typeid(variable).name() */
};

void State_set_context(State_t *pThis, Context_t *context) {
	pThis->context_ = context;
}

State_t *State_Init(State_t *pThis) {
	pThis->context_ = NULL;
	return pThis;
}

/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */

struct Context {
	State_t *state_;
};

void Context_TransitionTo(Context_t *pThis, State_t *state) {
	printf("Context: Transition to %s.\n", state->name(state));
	
	pThis->state_ = state;
	State_set_context(pThis->state_, pThis);
}

Context_t *Context_Init(Context_t *pThis, State_t *state) {
	Context_TransitionTo(pThis, state);
	return pThis;
}

void Context_Request1(Context_t *pThis) {
	pThis->state_->Handle1(pThis->state_);
}

void Context_Request2(Context_t *pThis) {
	pThis->state_->Handle2(pThis->state_);
}

typedef struct ConcreteStateA ConcreteStateA_t;
typedef struct ConcreteStateB ConcreteStateB_t;

struct ConcreteStateA {
	State_t state;
};

struct ConcreteStateB {
	State_t state;
};

/* instances of states */
ConcreteStateA_t g_ConcreteStateA;
ConcreteStateB_t g_ConcreteStateB;

void ConcreteStateA_Handle1(ConcreteStateA_t *pThis) {
	printf("ConcreteStateA handles request1.\n");
    printf("ConcreteStateA wants to change the state of the context.\n");
	
	Context_TransitionTo(pThis->state.context_, (State_t *)&g_ConcreteStateB);
}

void ConcreteStateA_Handle2(ConcreteStateA_t *pThis) {
	printf("ConcreteStateA handles request2.\n");
}

const uint8_t *ConcreteStateA_name(ConcreteStateA_t *pThis) {
	return "ConcreteStateA";
}

ConcreteStateA_t *ConcreteStateA_Init(ConcreteStateA_t *pThis) {
	State_Init((State_t *)pThis); /* type casting - parent interface class has a member variable */
	pThis->state.Handle1 = (void (*)(State_t *))ConcreteStateA_Handle1;
	pThis->state.Handle2 = (void (*)(State_t *))ConcreteStateA_Handle2;
	pThis->state.name = (const uint8_t * (*)(State_t *))ConcreteStateA_name;
	
	return pThis;
}

void ConcreteStateB_Handle1(ConcreteStateB_t *pThis) {
	printf("ConcreteStateB handles request1.\n");
}

void ConcreteStateB_Handle2(ConcreteStateB_t *pThis) {
	printf("ConcreteStateB handles request2.\n");
    printf("ConcreteStateB wants to change the state of the context.\n");
	
	Context_TransitionTo(pThis->state.context_, (State_t *)&g_ConcreteStateA);
}

const uint8_t *ConcreteStateB_name(ConcreteStateB_t *pThis) {
	return "ConcreteStateB";
}

ConcreteStateB_t *ConcreteStateB_Init(ConcreteStateB_t *pThis) {
	State_Init((State_t *)pThis); /* type casting - parent interface class has a member variable */
	pThis->state.Handle1 = (void (*)(State_t *))ConcreteStateB_Handle1;
	pThis->state.Handle2 = (void (*)(State_t *))ConcreteStateB_Handle2;
	pThis->state.name = (const uint8_t * (*)(State_t *))ConcreteStateB_name;
	
	return pThis;
}

void ClientCode() {
  ConcreteStateA_Init(&g_ConcreteStateA); /* initialize the instance */
  ConcreteStateB_Init(&g_ConcreteStateB); /* initialize the instance */
  Context_t context;
  Context_Init(&context, (State_t *)&g_ConcreteStateA); /* type casting */
  Context_Request1(&context);
  Context_Request2(&context);
}

int main() {
  ClientCode();
  return 0;
}
