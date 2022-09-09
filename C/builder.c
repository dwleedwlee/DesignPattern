/* Wher there are many different configurations of the product */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "vector.h"

typedef struct Product1{
	vector parts_;
}Product1_t;

Product1_t *Product1_Init(Product1_t *pThis) {
	vector_init(&(pThis->parts_));
	return pThis;
}
	
void Product1_ListParts(Product1_t *pThis) {	
	printf("Product parts: ");
	
	size_t i;
	vector *p_parts = &(pThis->parts_);
	uint32_t size = p_parts->pfVectorTotal(p_parts);
	for (i = 0; i < size; i++) {
		uint8_t *str = p_parts->pfVectorGet(p_parts, i);
		if(i == (size - 1)) {
			printf("%s", str);			
		} else {
			printf("%s, ", str);
		}		
	}
	printf("\n\n");
}

/* Actual product object */
Product1_t g_product1;

typedef struct Builder Builder_t;

struct Builder{
    void (* ProducePartA)(Builder_t *pThis);
    void (* ProducePartB)(Builder_t *pThis);
    void (* ProducePartC)(Builder_t *pThis);
};

typedef struct ConcreteBuilder1 {
	Builder_t builder;	/* Interface Inheritance */
	
	Product1_t *product;
}ConcreteBuilder1_t;

void ConcreteBuilder1_Reset(ConcreteBuilder1_t *pThis) {
	memset(&g_product1, 0, sizeof(g_product1));	/* Initialize the memory space of the actual product object */
	pThis->product = Product1_Init(&g_product1); /* Did not allocate new memory space */
}

void ConcreteBuilder1_ProducePartA(ConcreteBuilder1_t *pThis) {
	pThis->product->parts_.pfVectorAdd(&(pThis->product->parts_), "PartA1");
}

void ConcreteBuilder1_ProducePartB(ConcreteBuilder1_t *pThis) {
	pThis->product->parts_.pfVectorAdd(&(pThis->product->parts_), "PartB1");
}

void ConcreteBuilder1_ProducePartC(ConcreteBuilder1_t *pThis) {
	pThis->product->parts_.pfVectorAdd(&(pThis->product->parts_), "PartC1");
}

ConcreteBuilder1_t *ConcreteBuilder1_Init(ConcreteBuilder1_t *pThis) {
	ConcreteBuilder1_Reset(pThis);
	
	pThis->builder.ProducePartA = (void (*)(Builder_t *))ConcreteBuilder1_ProducePartA;	/* type casting */
	pThis->builder.ProducePartB = (void (*)(Builder_t *))ConcreteBuilder1_ProducePartB;
	pThis->builder.ProducePartC = (void (*)(Builder_t *))ConcreteBuilder1_ProducePartC;
}

Product1_t *ConcreteBuilder1_GetProduct(ConcreteBuilder1_t *pThis) {
	/* Do not reset */
	return pThis->product;
}

typedef struct Director {
	Builder_t *builder;
}Director_t;

void Director_set_builder(Director_t *pThis, Builder_t *builder) {
	pThis->builder = builder;
}

void Director_BuildMinimalViableProduct(Director_t *pThis) {
	pThis->builder->ProducePartA(pThis->builder);
}

void Director_BuildFullFeaturedProduct(Director_t *pThis) {
	pThis->builder->ProducePartA(pThis->builder);
	pThis->builder->ProducePartB(pThis->builder);
	pThis->builder->ProducePartC(pThis->builder);
}

void ClientCode(Director_t *director) {
	ConcreteBuilder1_t builder;
	ConcreteBuilder1_Init(&builder);	/* Constructor */
	
	Director_set_builder(director, (Builder_t *)&builder); /* upcasting */
	
	Product1_t *p;
	printf("Standard basic product:\n");	
	Director_BuildMinimalViableProduct(director);	
	p = ConcreteBuilder1_GetProduct(&builder);
	Product1_ListParts(p);	
	ConcreteBuilder1_Reset(&builder);	/* initialize */
	
	printf("Standard full featured product:\n");
	Director_BuildFullFeaturedProduct(director);
	p = ConcreteBuilder1_GetProduct(&builder);
	Product1_ListParts(p);
	ConcreteBuilder1_Reset(&builder);
	
	// Remember, the Builder pattern can be used without a Director class.
    printf("Custom product:\n");
    builder.builder.ProducePartA((Builder_t *)&builder); /* upcasting */
    builder.builder.ProducePartC((Builder_t *)&builder); /* upcasting */
    p = ConcreteBuilder1_GetProduct(&builder);
    Product1_ListParts(p);
}

int main(void){
    Director_t director;
    ClientCode(&director);
    return 0;    
}
	