
/* cache, sharing parts of object */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "dict.h"

#define NUM_FLYWEIGHT_OBJECT	10

typedef struct SharedState {
    uint8_t *brand_;
    uint8_t *model_;
    uint8_t *color_;
}SharedState_t;

SharedState_t *SharedState_Init(SharedState_t *pThis, uint8_t *brand, uint8_t *model, uint8_t *color) {
	pThis->brand_ = brand;
	pThis->model_ = model;
	pThis->color_ = color;
	return pThis;
}

uint8_t *SharedState_OperLeftShiftStr(SharedState_t *pThis) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s%s%s%s%s%s", "[ ", pThis->brand_, " , ", pThis->model_, " , ", pThis->color_, " ]");
	return buf;
}

typedef struct UniqueState {
    uint8_t *owner_;
    uint8_t *plates_;
}UniqueState_t;

UniqueState_t *UniqueState_Init(UniqueState_t *pThis, uint8_t *owner, uint8_t *plates) {
	pThis->owner_ = owner;
	pThis->plates_ = plates;
	return pThis;
}

uint8_t *UniqueState_OperLeftShiftStr(UniqueState_t *pThis) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s%s%s%s", "[ ", pThis->owner_, " , ", pThis->plates_, " ]");
	return buf;
}

typedef struct Flyweight {
	SharedState_t *shared_state_;
} Flyweight_t;

Flyweight_t *Flyweight_Init_SS(Flyweight_t *pThis, SharedState_t *shared_state) {
	pThis->shared_state_ = shared_state;	//shallow copy
}

Flyweight_t *Flyweight_Init_Fl(Flyweight_t *pThis, Flyweight_t *other) {
	pThis->shared_state_ = other->shared_state_;	//shallow copy
}

SharedState_t *Flyweight_shared_state(Flyweight_t *pThis) {
	return pThis->shared_state_;
}

void Flyweight_Operation(Flyweight_t *pThis, UniqueState_t *unique_state) {
	printf("%s%s%s%s%s", "Flyweight: Displaying shared (", 
		SharedState_OperLeftShiftStr(pThis->shared_state_),
		") and unique (",
		UniqueState_OperLeftShiftStr(unique_state),
		") state.\n");
}	

typedef struct FlyweightFactory {
	dict_t **flyweights_;
} FlyweightFactory_t;

uint8_t *FlyweightFactory_GetKey(FlyweightFactory_t *pThis, SharedState_t *ss) {
	/* key is combination of pointers of string */
	
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s%s%s%s", ss->brand_, "_", ss->model_, "_", ss->color_);
	return buf;
}

/* Flyweight instances */
size_t g_indexFlyweight = 0;
Flyweight_t g_flyweight[NUM_FLYWEIGHT_OBJECT];

/* n must less than NUM_FLYWEIGHT_OBJECT */
FlyweightFactory_t *FlyweightFactory_Init(FlyweightFactory_t *pThis, SharedState_t share_states[], size_t n) {
	pThis->flyweights_ = dictAlloc();
	
	size_t i = 0;
	for(; i < n; i++) {
		addItem(pThis->flyweights_, (char *)FlyweightFactory_GetKey(pThis, &share_states[i]), 
			Flyweight_Init_SS(&g_flyweight[i], &share_states[i]));
	}
	g_indexFlyweight = i;
}

Flyweight_t *FlyweightFactory_GetFlyweight(FlyweightFactory_t *pThis, SharedState_t *shared_state)
{	
	uint8_t *key = FlyweightFactory_GetKey(pThis, shared_state);
	Flyweight_t *pObj = getItem(*(pThis->flyweights_), key);
	if (pObj == NULL)
	{
		printf("FlyweightFactory: Can't find a flyweight, creating new one.\n");
		/* FIXME : need to create a function allocates and returns a flyweight instance */		
		addItem(pThis->flyweights_, key, Flyweight_Init_SS(&g_flyweight[g_indexFlyweight], shared_state));
		pObj = &g_flyweight[g_indexFlyweight];
		g_indexFlyweight++;
	}
	else
	{
		printf("FlyweightFactory: Reusing existing flyweight.\n");
	}
	return pObj;
}

void FlyweightFactory_ListFlyweights(FlyweightFactory_t *pThis)
{
	dict_t *pElement = *(pThis->flyweights_);
	size_t count = getSize(pElement);
	printf("\nFlyweightFactory: I have %d flyweights:\n", count);
	size_t i = 0;	
	/* FIXME : need to implement iterator */
	for(; pElement != NULL; pElement = pElement->next) {
		printf("%s\n", pElement->key); 
	}
}

void AddCarToPoliceDatabase(
    FlyweightFactory_t *ff, uint8_t *plates, uint8_t *owner,
    uint8_t *brand, uint8_t *model, uint8_t *color)
{
    printf("\nClient: Adding a car to database.\n");
    Flyweight_t *flyweight = FlyweightFactory_GetFlyweight(ff, &(SharedState_t){brand, model, color});

    // The client code either stores or calculates extrinsic state and passes it
    // to the flyweight's methods.
	Flyweight_Operation(flyweight, &(UniqueState_t){owner, plates});
}

int main()
{
    FlyweightFactory_t factory;
	SharedState_t shared[] = {
		{"Chevrolet", "Camaro2018", "pink"}, 
		{"Mercedes Benz", "C300", "black"}, 
		{"Mercedes Benz", "C500", "red"}, 
		{"BMW", "M5", "red"}, 
		{"BMW", "X6", "white"}};
		
	FlyweightFactory_Init(&factory, shared, 5);
		
    FlyweightFactory_ListFlyweights(&factory);

    AddCarToPoliceDatabase(&factory,
                            "CL234IR",
                            "James Doe",
                            "BMW",
                            "M5",
                            "red");
							
    AddCarToPoliceDatabase(&factory,
                            "CL234IR",
                            "James Doe",
                            "BMW",
                            "X1",
                            "red");
							
    FlyweightFactory_ListFlyweights(&factory);

    return 0;
}
	