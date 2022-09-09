/* creating objects instead of using a direct constructor call (new operator) */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Product Product_t;
typedef union uProduct uProduct_t;

struct Product {
	uint8_t *(* Operation)(uProduct_t *pThis);
};

typedef struct ConcreteProduct1 {
	Product_t product;
}ConcreteProduct1_t;

typedef struct ConcreteProduct2 {
	Product_t product;
}ConcreteProduct2_t;

union uProduct {
	Product_t product;
	ConcreteProduct1_t concreteProduct1;
	ConcreteProduct2_t concreteProduct2;
};

uint8_t *ConcreteProduct1_Operation(uProduct_t *pThis){
	return "{Result of the ConcreteProduct1}";
}

uint8_t *ConcreteProduct2_Operation(uProduct_t *pThis){
	return "{Result of the ConcreteProduct2}";
}

ConcreteProduct1_t *ConcreteProduct1_Init(ConcreteProduct1_t *pThis) {
	pThis->product.Operation = ConcreteProduct1_Operation;
	return pThis;
}

ConcreteProduct2_t *ConcreteProduct2_Init(ConcreteProduct2_t *pThis) {
	pThis->product.Operation = ConcreteProduct2_Operation;
	return pThis;
}

typedef struct Creator Creator_t;
typedef union uCreator uCreator_t;

struct Creator {
	uProduct_t *(* FactoryMethod)(uCreator_t *pThis);
};

typedef struct ConcreteCreator1 {
	Creator_t creator;
} ConcreteCreator1_t;

typedef struct ConcreteCreator2 {
	Creator_t creator;
} ConcreteCreator2_t;

union uCreator {
	Creator_t creator;
	ConcreteCreator1_t concreteCreator1;
	ConcreteCreator2_t concreteCreator2;
};

uint8_t *Creator_SomeOperation(uCreator_t *pThis) {
	uProduct_t *product = pThis->creator.FactoryMethod(pThis);
	
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s", "Creator: The same creator's code has just worked with ", product->product.Operation(product));
	return buf;
}

/* Actual product object */
ConcreteProduct1_t g_ConcreteProduct1;
ConcreteProduct2_t g_ConcreteProduct2;

uProduct_t *ConcreteCreator1_FactoryMethod(uCreator_t *pThis) {
	return (uProduct_t *)ConcreteProduct1_Init(&g_ConcreteProduct1); /* upcasting */
}

uProduct_t *ConcreteCreator2_FactoryMethod(uCreator_t *pThis) {
	return (uProduct_t *)ConcreteProduct2_Init(&g_ConcreteProduct2); /* upcasting */
}

ConcreteCreator1_t *ConcreteCreator1_Init(ConcreteCreator1_t *pThis) {
	pThis->creator.FactoryMethod = ConcreteCreator1_FactoryMethod;
	return pThis;
}

ConcreteCreator2_t *ConcreteCreator2_Init(ConcreteCreator2_t *pThis) {
	pThis->creator.FactoryMethod = ConcreteCreator2_FactoryMethod;
	return pThis;
}

void ClientCode(uCreator_t *creator) {
  // ...
  printf("Client: I'm not aware of the creator's class, but it still works.\n");
  printf("%s\n", Creator_SomeOperation(creator));
  // ...
}

int main() {
  printf("App: Launched with the ConcreteCreator1.\n");
  
  ConcreteCreator1_t creator;
  ConcreteCreator1_Init(&creator);  
  ClientCode((uCreator_t *)&creator); /* upcasting */
  
  printf("\n");
  printf("App: Launched with the ConcreteCreator2.\n");
  
  ConcreteCreator2_t creator2;
  ConcreteCreator2_Init(&creator2);  
  ClientCode((uCreator_t *)&creator2); /* upcasting */

  return 0;
}
