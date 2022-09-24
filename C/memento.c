/* make a snapshot of an object, undo, redo  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"

static uint8_t *substr(uint8_t *pStr, size_t start, size_t size) {
  /* FIXME : neet to allocate the buffer size statically */
  static uint8_t buf[256];
  memcpy(buf, pStr + start, size);
  buf[size] = '\0';
  return buf;
}

typedef struct Memento Memento_t;

/**
 * The Memento interface provides a way to retrieve the memento's metadata, such
 * as creation date or name. However, it doesn't expose the Originator's state.
 */
struct Memento {
	uint8_t *(*GetName)(Memento_t *pThis);
	uint8_t *(*date)(Memento_t *pThis);
	uint8_t *(*state)(Memento_t *pThis);
};

/**
 * The Concrete Memento contains the infrastructure for storing the Originator's
 * state.
 */
typedef struct ConcreteMemento {
	Memento_t memento;
	
	uint8_t *state_;
	uint8_t *date_;
}ConcreteMemento_t;

/**
* The Originator uses this method when restoring its state.
*/
uint8_t *ConcreteMemento_state(ConcreteMemento_t *pThis) {
	return pThis->state_;
}

/**
* The rest of the methods are used by the Caretaker to display metadata.
*/
uint8_t *ConcreteMemento_GetName(ConcreteMemento_t *pThis) {
  /* FIXME : neet to allocate the buffer size statically */
  static uint8_t buf[256];
  sprintf(buf, "%s%s%s%s", pThis->date_, " / (", substr(pThis->state_, 0, 9), "...)");
  return buf;	
}

uint8_t *ConcreteMemento_date(ConcreteMemento_t *pThis)	{
	return pThis->date_;
}

ConcreteMemento_t *ConcreteMemento_Init(ConcreteMemento_t *pThis, uint8_t *state) {
	pThis->memento.GetName = (uint8_t *(*)(Memento_t *))ConcreteMemento_GetName;
	pThis->memento.date = (uint8_t *(*)(Memento_t *))ConcreteMemento_date;
	pThis->memento.state = (uint8_t *(*)(Memento_t *))ConcreteMemento_state;
	
	pThis->state_ = state;
	time_t now;
	time(&now);
	pThis->date_ = ctime(&now);
}

#define NUM_MAX_MEMENTO_INDEX	10

/* instances of ConcreteMemento class */
ConcreteMemento_t g_ConMemento[NUM_MAX_MEMENTO_INDEX];

/* instances of random string */
/* FIXME : neet to allocate the buffer size statically */
static uint8_t g_randStr[NUM_MAX_MEMENTO_INDEX][256];
 
/**
 * The Originator holds some important state that may change over time. It also
 * defines a method for saving the state inside a memento and another method for
 * restoring the state from it.
 */
typedef struct Originator {
	/**
	* @var string For the sake of simplicity, the originator's state is stored
	* inside a single variable.
	*/
	uint8_t *state_;
	
	size_t indexMem; /* index of ConcreteMemento */
	size_t indexRanStr; /* index of RandomString */
} Originator_t;

uint8_t *Originator_GenerateRandomString(Originator_t *pThis, size_t length) {
	const uint8_t alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	size_t stringLength = sizeof(alphanum) - 1;
	int i = 0;
   
	for (; i < length; i++) {
	  sprintf(g_randStr[pThis->indexRanStr] + i, "%c", alphanum[rand() % stringLength]);
	}
	uint8_t *random_string = g_randStr[pThis->indexRanStr++];
	return random_string;
}

Originator_t *Originator_Init(Originator_t *pThis, uint8_t *state) {
	pThis->state_ = state;
	pThis->indexMem = 0;
	pThis->indexRanStr = 0;
	printf("Originator: My initial state is: %s\n", pThis->state_);
	return pThis;
}

/**
* The Originator's business logic may affect its internal state. Therefore,
* the client should backup the state before launching methods of the business
* logic via the save() method.
*/
void Originator_DoSomething(Originator_t *pThis) {
	printf("Originator: I'm doing something important.\n");
	pThis->state_ = Originator_GenerateRandomString(pThis, 30);
	printf("Originator: and my state has changed to: %s\n", pThis->state_);
}

/**
* Saves the current state inside a memento.
*/
Memento_t *Originator_Save(Originator_t *pThis) {
	if(pThis->indexMem <= NUM_MAX_MEMENTO_INDEX) { /* max number of instance */
		return (Memento_t *)ConcreteMemento_Init(&g_ConMemento[pThis->indexMem++], pThis->state_);
	} else {
		return NULL;
	}
}

/**
* Restores the Originator's state from a memento object.
*/
void Originator_Restore(Originator_t *pThis, Memento_t *memento) {
	pThis->state_ = memento->state(memento);
	printf("Originator: My state has changed to: %s\n", pThis->state_);
}

/**
 * The Caretaker doesn't depend on the Concrete Memento class. Therefore, it
 * doesn't have access to the originator's state, stored inside the memento. It
 * works with all mementos via the base Memento interface.
 */
typedef struct Caretaker {
	vector mementos_;
	Originator_t *originator_;
} Caretaker_t;	
	
Caretaker_t *Caretaker_Init(Caretaker_t *pThis, Originator_t *originator) {
	vector_init(&(pThis->mementos_));
	pThis->originator_ = originator;
	return pThis;
}

void Caretaker_Backup(Caretaker_t *pThis) {
	printf("\nCaretaker: Saving Originator's state...\n");
	pThis->mementos_.pfVectorAdd(&(pThis->mementos_), Originator_Save(pThis->originator_));
}

void Caretaker_Undo(Caretaker_t *pThis) {
	int total = pThis->mementos_.pfVectorTotal(&(pThis->mementos_));
	if (!total) {
	  return;
	}
	Memento_t *memento = pThis->mementos_.pfVectorGet(&(pThis->mementos_), total - 1);
	pThis->mementos_.pfVectorGet(&(pThis->mementos_), total - 1);
	pThis->mementos_.pfVectorDelete(&(pThis->mementos_), total - 1);
	printf("Caretaker: Restoring state to: %s\n", memento->GetName(memento));

	/* Did not implement try ~ catch statement */
	Originator_Restore(pThis->originator_, memento);
}

void Caretaker_ShowHistory(Caretaker_t *pThis) {
	printf("Caretaker: Here's the list of mementos:\n");
	int total = pThis->mementos_.pfVectorTotal(&(pThis->mementos_));
	int i = 0;
	for (; i < total; i++) {
		Memento_t *memento = pThis->mementos_.pfVectorGet(&(pThis->mementos_), i);
		printf("%s\n", memento->GetName(memento));
	}
}

/**
 * Client code.
 */

void ClientCode() {
  Originator_t originator;
  Originator_Init(&originator, "Super-duper-super-puper-super.");
  Caretaker_t caretaker;
  Caretaker_Init(&caretaker, &originator);
  Caretaker_Backup(&caretaker);
  Originator_DoSomething(&originator);
  Caretaker_Backup(&caretaker);
  Originator_DoSomething(&originator);
  Caretaker_Backup(&caretaker);
  Originator_DoSomething(&originator);
  printf("\n");
  Caretaker_ShowHistory(&caretaker);
  printf("\nClient: Now, let's rollback!\n\n");
  Caretaker_Undo(&caretaker);
  printf("\nClient: Once more!\n\n");
  Caretaker_Undo(&caretaker);
}

int main() {
  srand((unsigned int)time(NULL));
  ClientCode();
  return 0;
}
