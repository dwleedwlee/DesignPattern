/* object of interchangeable behaviors  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

typedef struct Strategy Strategy_t;

struct Strategy {
	uint8_t *(* doAlgorithm)(Strategy_t *pThis, uint8_t *data, size_t n);
};

typedef struct Context {
	Strategy_t *strategy_; /* Cannot use a smart pointer */
} Context_t;
	
	
Context_t *Context_Init(Context_t *pThis, Strategy_t *strategy) {
	pThis->strategy_ = strategy;
	return pThis;
}

void Context_set_strategy(Context_t *pThis, Strategy_t *strategy) {
	pThis->strategy_ = strategy;
}

/**
 * The Context delegates some work to the Strategy object instead of
 * implementing +multiple versions of the algorithm on its own.
 */
void Context_doSomeBusinessLogic(Context_t *pThis) {
	if (pThis->strategy_) {
		printf("Context: Sorting data using the strategy (not sure how it'll do it)\n");
		uint8_t *result = pThis->strategy_->doAlgorithm(pThis->strategy_, "aecbd", strlen("aecbd"));
		printf("%s\n", result);
	} else {
		printf("Context: Strategy isn't set\n");
	}
}

int compAscend(const void *a, const void *b)
{
    char val1 = *(char *)a;
    char val2 = *(char *)b;

    if (val1 < val2)
        return -1;
    
    if (val1 > val2)
        return 1;
    
    return 0;
}

int compDescend(const void *a, const void *b)
{
    char val1 = *(char *)a;
    char val2 = *(char *)b;

    if (val1 > val2)
        return -1;
    
    if (val1 < val2)
        return 1;
    
    return 0;
}

/**
 * Concrete Strategies implement the algorithm while following the base Strategy
 * interface. The interface makes them interchangeable in the Context.
 */
typedef struct ConcreteStrategyA {
	Strategy_t strategy;	
}ConcreteStrategyA_t;

uint8_t *ConcreteStrategyA_doAlgorithm(ConcreteStrategyA_t *pThis, uint8_t *data, size_t n) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256] = {'\0', };
	if(n < 256) {		
		memcpy(buf, data, n); /* n < 256 */
		buf[n+1] = '\0';
		qsort(buf, n, sizeof(uint8_t), compAscend);
	}	
	return buf;
}

ConcreteStrategyA_t *ConcreteStrategyA_Init(ConcreteStrategyA_t *pThis) {
	pThis->strategy.doAlgorithm = (uint8_t *(*)(Strategy_t *, uint8_t *, size_t))ConcreteStrategyA_doAlgorithm;
	return pThis;
}

typedef struct ConcreteStrategyB {
	Strategy_t strategy;	
}ConcreteStrategyB_t;

uint8_t *ConcreteStrategyB_doAlgorithm(ConcreteStrategyB_t *pThis, uint8_t *data, size_t n) {
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t buf[256] = {'\0', };
	if(n < 256) {		
		memcpy(buf, data, n); /* n < 256 */
		buf[n+1] = '\0';
		qsort(buf, n, sizeof(uint8_t), compDescend);
	}	
	return buf;
}

ConcreteStrategyB_t *ConcreteStrategyB_Init(ConcreteStrategyB_t *pThis) {
	pThis->strategy.doAlgorithm = (uint8_t *(*)(Strategy_t *, uint8_t *, size_t))ConcreteStrategyB_doAlgorithm;
	return pThis;
}

void clientCode()
{
    Context_t context;
	ConcreteStrategyA_t strategyA;
	ConcreteStrategyA_Init(&strategyA);
	Context_Init(&context, (Strategy_t *)&strategyA);	/* type casting */
    printf("Client: Strategy is set to normal sorting.\n");
    Context_doSomeBusinessLogic(&context);
    printf("\n");
    printf("Client: Strategy is set to reverse sorting.\n");
	ConcreteStrategyB_t strategyB;
	ConcreteStrategyB_Init(&strategyB);	
	Context_set_strategy(&context, (Strategy_t *)&strategyB);	/* type casting */
    Context_doSomeBusinessLogic(&context);
}

int main()
{
    clientCode();
    return 0;
}
