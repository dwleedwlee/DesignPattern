/* sequential traversal through a complex data structure  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "vector.h"

typedef struct Data {
	int m_data_;
} Data_t;

Data_t* Data_Init(Data_t* pThis, int a) {
	pThis->m_data_ = a;
	return pThis;
}

void Data_set_data(Data_t* pThis, int a) {
	pThis->m_data_ = a;
}

int Data_data(Data_t* pThis) {
	return pThis->m_data_;
}

typedef struct Container {
	vector m_data_;
}Container_t;

typedef struct Iterator {
	Container_t *m_p_data_;
	int m_index;	/* custom vector does not have iterator, substituted it as index of vector */
} Iterator_t;

Iterator_t *Iterator_Init(Iterator_t *pThis, Container_t *p_data) {
	pThis->m_p_data_ = p_data;
	pThis->m_index = 0;
	return pThis;
}

void Iterator_First(Iterator_t *pThis) { /* reset */
	pThis->m_index = 0;
}

void Iterator_Next(Iterator_t *pThis) {
	vector *pVec = &(pThis->m_p_data_->m_data_);
	if(pThis->m_index < pVec->pfVectorTotal(pVec)){ pThis->m_index++; }
}

bool Iterator_IsDone(Iterator_t *pThis) {
	vector *pVec = &(pThis->m_p_data_->m_data_);
	return (pThis->m_index == pVec->pfVectorTotal(pVec));
}

int Iterator_Current(Iterator_t *pThis) {
	return pThis->m_index;
}

/* Single iterator instance */
Iterator_t g_iterator;

void Container_Add_int(Container_t *pThis, int *a) {
	pThis->m_data_.pfVectorAdd(&(pThis->m_data_), a);
}

void Container_Add_Data(Container_t *pThis, Data_t *a) {
	pThis->m_data_.pfVectorAdd(&(pThis->m_data_), a);
}

Iterator_t *Container_CreateIterator(Container_t *pThis) {
	return Iterator_Init(&g_iterator, pThis);
}

Container_t *Container_Init(Container_t *pThis) {
	vector_init(&(pThis->m_data_));
	return pThis;
}

#define NUM_INTEGER_DATA	(10)

void ClientCode() {
  printf("________________Iterator with int______________________________________\n");
  Container_t cont;
  Container_Init(&cont);
  
  static int data[NUM_INTEGER_DATA]; /* Allocate static memory area for integer data */
  int i = 0;
  for (; i < NUM_INTEGER_DATA; i++) {
	data[i] = i; 
    Container_Add_int(&cont, &data[i]);
  }

  Iterator_t *it = Container_CreateIterator(&cont);
  for (Iterator_First(it); !Iterator_IsDone(it); Iterator_Next(it)) {
	vector *pVec = &(it->m_p_data_->m_data_);
    printf("%d\n", *(int *)(pVec->pfVectorGet(pVec, Iterator_Current(it))) );
  }
  

  Container_t cont2;
  Container_Init(&cont2);
  Data_t a, b, c;
  Data_Init(&a, 100);
  Data_Init(&b, 1000);
  Data_Init(&c, 10000);

  Container_Add_Data(&cont2, &a);
  Container_Add_Data(&cont2, &b);
  Container_Add_Data(&cont2, &c);

  printf("________________Iterator with custom Class______________________________\n");
  Iterator_t *it2 = Container_CreateIterator(&cont2);  
  for (Iterator_First(it2); !Iterator_IsDone(it2); Iterator_Next(it2)) {
	vector *pVec = &(it->m_p_data_->m_data_);
    printf("%d\n", Data_data((Data_t *)(pVec->pfVectorGet(pVec, Iterator_Current(it)))) );
  }
}

int main() {
  ClientCode();
  return 0;
}
	