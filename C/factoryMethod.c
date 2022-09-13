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

/* creating objects instead of using a direct constructor call (new operator) */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Product Product_t;

struct Product {
	uint8_t *(* Operation)(Product_t *pThis); /* Argument type may be void */
};

typedef struct ConcreteProduct1 {
	Product_t product;
}ConcreteProduct1_t;

typedef struct ConcreteProduct2 {
	Product_t product;
}ConcreteProduct2_t;

uint8_t *ConcreteProduct1_Operation(ConcreteProduct1_t *pThis){
	return "{Result of the ConcreteProduct1}";
}

uint8_t *ConcreteProduct2_Operation(ConcreteProduct2_t *pThis){
	return "{Result of the ConcreteProduct2}";
}

ConcreteProduct1_t *ConcreteProduct1_Init(ConcreteProduct1_t *pThis) {
	pThis->product.Operation = (uint8_t *(*)(Product_t *))ConcreteProduct1_Operation;	 /* type casting */
	return pThis;
}

ConcreteProduct2_t *ConcreteProduct2_Init(ConcreteProduct2_t *pThis) {
	pThis->product.Operation = (uint8_t *(*)(Product_t *))ConcreteProduct2_Operation;
	return pThis;
}

typedef struct Creator Creator_t;

struct Creator {
	Product_t *(* FactoryMethod)(Creator_t *pThis);
};

uint8_t *Creator_SomeOperation(Creator_t *pThis) {
	Product_t *product = pThis->FactoryMethod(pThis);
	
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	sprintf(buf, "%s%s", "Creator: The same creator's code has just worked with ", product->Operation(product));
	return buf;
}

typedef struct ConcreteCreator1 {
	Creator_t creator;
} ConcreteCreator1_t;

typedef struct ConcreteCreator2 {
	Creator_t creator;
} ConcreteCreator2_t;

/* Actual product object */
ConcreteProduct1_t g_ConcreteProduct1;
ConcreteProduct2_t g_ConcreteProduct2;

Product_t *ConcreteCreator1_FactoryMethod(ConcreteCreator1_t *pThis) {
	return (Product_t *)ConcreteProduct1_Init(&g_ConcreteProduct1); /* upcasting */
}

Product_t *ConcreteCreator2_FactoryMethod(ConcreteCreator1_t *pThis) {
	return (Product_t *)ConcreteProduct2_Init(&g_ConcreteProduct2); /* upcasting */
}

ConcreteCreator1_t *ConcreteCreator1_Init(ConcreteCreator1_t *pThis) {
	pThis->creator.FactoryMethod = (Product_t *(*)(Creator_t *))ConcreteCreator1_FactoryMethod; /* type casting */
	return pThis;
}

ConcreteCreator2_t *ConcreteCreator2_Init(ConcreteCreator2_t *pThis) {
	pThis->creator.FactoryMethod = (Product_t *(*)(Creator_t *))ConcreteCreator2_FactoryMethod;
	return pThis;
}

void ClientCode(Creator_t* creator) {
  // ...
  printf("Client: I'm not aware of the creator's class, but it still works.\n");
  printf("%s\n", Creator_SomeOperation(creator));
  // ...
}

int main() {
  printf("App: Launched with the ConcreteCreator1.\n");
  
  ConcreteCreator1_t creator;
  ConcreteCreator1_Init(&creator);  
  ClientCode((Creator_t*)&creator); /* upcasting */
  
  printf("\n");
  printf("App: Launched with the ConcreteCreator2.\n");
  
  ConcreteCreator2_t creator2;
  ConcreteCreator2_Init(&creator2);  
  ClientCode((Creator_t*)&creator2); /* upcasting */

  return 0;
}
