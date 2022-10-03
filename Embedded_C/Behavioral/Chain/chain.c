
/* passing request along the chain, ex) filter, event chain */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "vector.h"

typedef struct Handler Handler_t;

struct Handler {
	Handler_t *(*SetNext)(Handler_t *pThis, Handler_t *handler);
	uint8_t *(*Handle)(Handler_t *pThis, uint8_t *request);
};

typedef struct AbstractHandler AbstractHandler_t;

struct AbstractHandler {
	Handler_t handle;
	
	Handler_t *next_handler_;
};

Handler_t *AbstractHandler_SetNext(AbstractHandler_t *pThis, Handler_t *handler) {
	pThis->next_handler_ = handler;
	return handler;
}

uint8_t *AbstractHandler_Handle(AbstractHandler_t *pThis, uint8_t *request) {
	if(pThis->next_handler_) {
		return pThis->next_handler_->Handle(pThis->next_handler_, request);
	}
	return NULL;
}

AbstractHandler_t AbstractHandler_Init(AbstractHandler_t *pThis) {
	pThis->handle.SetNext = (Handler_t *(*)(Handler_t *, Handler_t *))AbstractHandler_SetNext;
	pThis->handle.Handle = (uint8_t *(*)(Handler_t *, uint8_t *))AbstractHandler_Handle;
	pThis->next_handler_ = NULL;
}

typedef struct MonkeyHandler {
	AbstractHandler_t absHandle;
} MonkeyHandler_t;

uint8_t *MonkeyHandler_Handle(MonkeyHandler_t *pThis, uint8_t *request) {
	if(strcmp(request, "Banana") == 0) {
	  /* FIXME : neet to allocate the buffer size statically */
	  static uint8_t buf[256];
	  sprintf(buf, "%s%s%s", "Monkey: I'll eat the ", request, ".\n");
	  return buf;
    } else {
      return AbstractHandler_Handle((AbstractHandler_t *)pThis, request); /* upcasting */
    }
}

MonkeyHandler_t MonkeyHandler_Init(MonkeyHandler_t *pThis) {
	AbstractHandler_Init((AbstractHandler_t *)pThis); /* upcasting */ /* call the initializer of the parent */
	pThis->absHandle.handle.Handle = (uint8_t *(*)(Handler_t *, uint8_t *))MonkeyHandler_Handle;
}

typedef struct SquirrelHandler {
	AbstractHandler_t absHandle;
} SquirrelHandler_t;

uint8_t *SquirrelHandler_Handle(SquirrelHandler_t *pThis, uint8_t *request) {
	if(strcmp(request, "Nut") == 0) {
	  /* FIXME : neet to allocate the buffer size statically */
	  static uint8_t buf[256];
	  sprintf(buf, "%s%s%s", "Squirrel: I'll eat the ", request, ".\n");
	  return buf;
    } else {
      return AbstractHandler_Handle((AbstractHandler_t *)pThis, request); /* upcasting */
    }
}

SquirrelHandler_t SquirrelHandler_Init(SquirrelHandler_t *pThis) {
	AbstractHandler_Init((AbstractHandler_t *)pThis); /* upcasting */
	pThis->absHandle.handle.Handle = (uint8_t *(*)(Handler_t *, uint8_t *))SquirrelHandler_Handle;
}

typedef struct DogHandler {
	AbstractHandler_t absHandle;
} DogHandler_t;

uint8_t *DogHandler_Handle(DogHandler_t *pThis, uint8_t *request) {
	if(strcmp(request, "MeatBall") == 0) {
	  /* FIXME : neet to allocate the buffer size statically */
	  static uint8_t buf[256];
	  sprintf(buf, "%s%s%s", "Dog: I'll eat the ", request, ".\n");
	  return buf;
    } else {
      return AbstractHandler_Handle((AbstractHandler_t *)pThis, request); /* upcasting */
    }
}

DogHandler_t DogHandler_Init(DogHandler_t *pThis) {
	AbstractHandler_Init((AbstractHandler_t *)pThis); /* upcasting */
	pThis->absHandle.handle.Handle = (uint8_t *(*)(Handler_t *, uint8_t *))DogHandler_Handle;
}

void ClientCode(Handler_t *handler) {	
	int i = 0;
	VECTOR_INIT(food);
	food.pfVectorAdd(&food,"Nut");
    food.pfVectorAdd(&food,"Banana");
    food.pfVectorAdd(&food,"Cup of coffee");
	for (i = 0; i < food.pfVectorTotal(&food); i++)
    {
		uint8_t *f = (uint8_t *)food.pfVectorGet(&food, i);
        printf("Client: Who wants a %s?\n", f);
		const uint8_t *result = handler->Handle(handler, f); /* just refered */
		if (result != NULL) {
		    printf("  %s", result);
		} else {
			printf("  %s was left untouched.\n", f);
		}
    }
}

int main() {
  MonkeyHandler_t monkey;
  MonkeyHandler_Init(&monkey);
  SquirrelHandler_t squirrel;
  SquirrelHandler_Init(&squirrel);
  DogHandler_t dog;
  DogHandler_Init(&dog);
  
  Handler_t *handleMonkey = (Handler_t *)&monkey; /* upcasting */
  Handler_t *handleSquirrel = handleMonkey->SetNext(handleMonkey, (Handler_t *)&squirrel);
  handleSquirrel->SetNext(handleSquirrel, (Handler_t *)&dog);

  /**
   * The client should be able to send a request to any handler, not just the
   * first one in the chain.
   */
  printf("Chain: Monkey > Squirrel > Dog\n\n");
  ClientCode(handleMonkey);
  printf("\n");
  printf("Subchain: Squirrel > Dog\n\n");
  ClientCode(handleSquirrel);

  return 0;
}
	