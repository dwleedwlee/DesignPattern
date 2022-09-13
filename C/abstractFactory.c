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

/* leaves the actual product creation to concrete factory classes */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/* ProductA class */
typedef struct AbstractProductA AbstractProductA_t;

struct AbstractProductA {
	/* first argument of the member function is a pointer of the object itself to access the member variables */
  uint8_t *(* UsefulFunctionA)(AbstractProductA_t *);
  /* It does not have destructor because the object does not be created on heap */
};

typedef struct ConcreteProductA1 {
	AbstractProductA_t absProd;	/* Interface Inheritance */
}ConcreteProductA1_t;	

typedef struct ConcreteProductA2 {
	AbstractProductA_t absProd;
}ConcreteProductA2_t;

uint8_t *ConcreteProductA1_UsefulFunctionA(ConcreteProductA1_t *pThis) {
    return "The result of the product A1.";
}	

uint8_t *ConcreteProductA2_UsefulFunctionA(ConcreteProductA2_t *pThis) {
    return "The result of the product A2.";
}

/* Init function - Role of the constructor, initialize override functions and member variables */
ConcreteProductA1_t *InitContreteProductA1(ConcreteProductA1_t *pThis) {
	pThis->absProd.UsefulFunctionA = (uint8_t *(*)(AbstractProductA_t *))ConcreteProductA1_UsefulFunctionA;	/* type casting */
	return pThis;
}

ConcreteProductA2_t *InitContreteProductA2(ConcreteProductA2_t *pThis) {
	pThis->absProd.UsefulFunctionA = (uint8_t *(*)(AbstractProductA_t *))ConcreteProductA2_UsefulFunctionA;	/* type casting */
	return pThis;
}

/* ProductB class */
typedef struct AbstractProductB AbstractProductB_t;

struct AbstractProductB {
  uint8_t *(* UsefulFunctionB)(AbstractProductB_t *);
  uint8_t *(* AnotherUsefulFunctionB)(AbstractProductB_t *, AbstractProductA_t *);
};

typedef struct ConcreteProductB1 {
	AbstractProductB_t absProd;
}ConcreteProductB1_t;	

typedef struct ConcreteProductB2 {
	AbstractProductB_t absProd;
}ConcreteProductB2_t;

uint8_t *ConcreteProductB1_UsefulFunctionB(AbstractProductB_t *pThis) {
    return "The result of the product B1.";
}	

uint8_t *ConcreteProductB2_UsefulFunctionB(AbstractProductB_t *pThis) {
    return "The result of the product B2.";
}

uint8_t *ConcreteProductB1_AnotherUsefulFunctionB(AbstractProductB_t *pThis, AbstractProductA_t *pCollaborator) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256];
	const uint8_t *pResult = pCollaborator->UsefulFunctionA(pCollaborator);
	sprintf(buf, "%s%s%s", "The result of the B1 collaborating with ( ", pResult, ")");
	return buf;
}

uint8_t *ConcreteProductB2_AnotherUsefulFunctionB(AbstractProductB_t *pThis, AbstractProductA_t *pCollaborator) {
	static uint8_t buf[256];
	const uint8_t *pResult = pCollaborator->UsefulFunctionA(pCollaborator);
	sprintf(buf, "%s%s%s", "The result of the B2 collaborating with ( ", pResult, ")");
	return buf;
}

ConcreteProductB1_t *InitContreteProductB1(ConcreteProductB1_t *pThis) {
	pThis->absProd.UsefulFunctionB = ConcreteProductB1_UsefulFunctionB;
	pThis->absProd.AnotherUsefulFunctionB = ConcreteProductB1_AnotherUsefulFunctionB;
	return pThis;
}

ConcreteProductB2_t *InitContreteProductB2(ConcreteProductB2_t *pThis) {
	pThis->absProd.UsefulFunctionB = ConcreteProductB2_UsefulFunctionB;
	pThis->absProd.AnotherUsefulFunctionB = ConcreteProductB2_AnotherUsefulFunctionB;
	return pThis;
}


/* Factory class */
typedef struct AbstractFactory AbstractFactory_t;

struct AbstractFactory {
  AbstractProductA_t *(* CreateProductA)(AbstractFactory_t *pThis);
  AbstractProductB_t *(* CreateProductB)(AbstractFactory_t *pThis);
};

typedef struct ConcreteFactory1 {
	AbstractFactory_t absFact;
}ConcreteFactory1_t;

typedef struct ConcreteFactory2 {
	AbstractFactory_t absFact;
}ConcreteFactory2_t;

/* Embedded System does not allow dynamic memory allocation, so it uses the .data memory section instead of heap */
/* Actual product objects */
ConcreteProductA1_t gConcreteProductA1;
ConcreteProductA2_t gConcreteProductA2;
ConcreteProductB1_t gConcreteProductB1;
ConcreteProductB2_t gConcreteProductB2;

ConcreteProductA1_t *ConcreteFactory1_CreateProductA(ConcreteProductA1_t *pThis) {
	return InitContreteProductA1(&gConcreteProductA1);
}

ConcreteProductB1_t *ConcreteFactory1_CreateProductB(ConcreteProductB1_t *pThis) {
	return InitContreteProductB1(&gConcreteProductB1);
}

ConcreteProductA2_t *ConcreteFactory2_CreateProductA(ConcreteProductA2_t *pThis) {
	return InitContreteProductA2(&gConcreteProductA2);
}

ConcreteProductB2_t *ConcreteFactory2_CreateProductB(ConcreteProductB2_t *pThis) {
	return InitContreteProductB2(&gConcreteProductB2);
}

ConcreteFactory1_t *InitConcreteFactory1(ConcreteFactory1_t *pThis) {
	pThis->absFact.CreateProductA = (AbstractProductA_t *(*)(AbstractFactory_t *))ConcreteFactory1_CreateProductA; /* type casting */
	pThis->absFact.CreateProductB = (AbstractProductB_t *(*)(AbstractFactory_t *))ConcreteFactory1_CreateProductB;
	return pThis;
}

ConcreteFactory2_t *InitConcreteFactory2(ConcreteFactory2_t *pThis) {
	pThis->absFact.CreateProductA = (AbstractProductA_t *(*)(AbstractFactory_t *))ConcreteFactory2_CreateProductA;
	pThis->absFact.CreateProductB = (AbstractProductB_t *(*)(AbstractFactory_t *))ConcreteFactory2_CreateProductB;
	return pThis;
}

void ClientCode(AbstractFactory_t *factory) {	/* remove const qualifier */
  AbstractProductA_t *product_a = factory->CreateProductA(factory); /* remove const qualifier (reference -> pointer) */
  AbstractProductB_t *product_b = factory->CreateProductB(factory); /* remove const qualifier (reference -> pointer) */
  printf("%s\n", product_b->UsefulFunctionB(product_b));
  printf("%s\n", product_b->AnotherUsefulFunctionB(product_b, product_a));
}	


int main(void) {
	/* We can use stack memory instead of heap for creating factory object */
	ConcreteFactory1_t ConcreteFactory1;
	ConcreteFactory2_t ConcreteFactory2;

	printf("%s", "Client: Testing client code with the first factory type:\n");
	ConcreteFactory1_t *f1 = InitConcreteFactory1(&ConcreteFactory1);
	ClientCode((AbstractFactory_t *)f1);	/* upcasting */
	printf("\n");
	printf("%s", "Client: Testing the same client code with the second factory type:\n");
	ConcreteFactory2_t *f2 = InitConcreteFactory2(&ConcreteFactory2);
	ClientCode((AbstractFactory_t *)f2);	/* upcasting */
	return 0;
}
