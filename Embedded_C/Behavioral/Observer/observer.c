/* notify other objects about changes  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "linkedlist.h"

typedef struct IObserver IObserver_t;

struct IObserver {
	void (*Destructor)(IObserver_t *pThis);
	void (*Update)(IObserver_t *pThis, const uint8_t *message_from_subject);
};

typedef struct ISubject ISubject_t;

struct ISubject {
	void (*Destructor)(ISubject_t *pThis);
	void (*Attach)(ISubject_t *pThis, IObserver_t *observer);
	void (*Detach)(ISubject_t *pThis, IObserver_t *observer);
	void (*Notify)(ISubject_t *pThis);
};

/**
 * The Subject owns some important state and notifies observers when the state
 * changes.
 */
typedef struct Subject {
	ISubject_t isub;
	
	Node *list_observer_;
	uint8_t *message_;
} Subject_t;

void Subject_Destructor(Subject_t *pThis) {
	printf("Goodbye, I was the Subject.\n");
}

void Subject_Attach(Subject_t *pThis, IObserver_t *observer) {
	pThis->list_observer_ = postInsert(pThis->list_observer_, (int)observer);
}

void Subject_Detach(Subject_t *pThis, IObserver_t *observer) {
	pThis->list_observer_ = deleteNode(pThis->list_observer_, (int)observer);
}

void Subject_HowManyObserver(Subject_t *pThis) {
	printf("There are %d observers in the list.\n", countNodes(pThis->list_observer_));
}

void Subject_Notify(Subject_t *pThis) {
	Node *nd = pThis->list_observer_;
	
	Subject_HowManyObserver(pThis);
	while(nd != NULL) {
		IObserver_t *observer = (IObserver_t *)nd->n; /* int -> IObserver * */
		observer->Update(observer, pThis->message_);
		nd = nd->next;
	}
}

Subject_t *Subject_Init(Subject_t *pThis) {
	pThis->isub.Destructor = (void (*)(ISubject_t *))Subject_Destructor;
	pThis->isub.Attach = (void (*)(ISubject_t *, IObserver_t *))Subject_Attach;
	pThis->isub.Detach = (void (*)(ISubject_t *, IObserver_t *))Subject_Detach;
	pThis->isub.Notify = (void (*)(ISubject_t *))Subject_Notify;
	
	pThis->list_observer_ = NULL;
	pThis->message_ = NULL;
	return pThis;
}

void Subject_CreateMessage(Subject_t *pThis, uint8_t *message) {
	pThis->message_ = message;
	Subject_Notify(pThis);
}

/**
* Usually, the subscription logic is only a fraction of what a Subject can
* really do. Subjects commonly hold some important business logic, that
* triggers a notification method whenever something important is about to
* happen (or after it).
*/
void Subject_SomeBusinessLogic(Subject_t *pThis) {
	pThis->message_ = "change message message";
	Subject_Notify(pThis);
	printf("I'm about to do some thing important\n");
}


static int g_Observer_static_number_ = 0;
typedef struct Observer {
	IObserver_t iobserver;	
	
	Subject_t *subject_;
	const uint8_t *message_from_subject_;
	int number_;
}Observer_t;

void Observer_Destructor(Observer_t *pThis) {
	printf("Goodbye, I was the Observer \"%d\".\n", pThis->number_);
}

void Observer_PrintInfo(Observer_t *pThis) {
	printf("Observer \"%d\": a new message is available --> %s\n", pThis->number_, pThis->message_from_subject_);
}

void Observer_Update(Observer_t *pThis, const uint8_t *message_from_subject) {
	pThis->message_from_subject_ = message_from_subject;
	Observer_PrintInfo(pThis);
}

Observer_t *Observer_Init(Observer_t *pThis, Subject_t *subject) {
	pThis->subject_ = subject;
	pThis->iobserver.Destructor = (void (*)(IObserver_t *))Observer_Destructor;
	pThis->iobserver.Update = (void (*)(IObserver_t *, const uint8_t *))Observer_Update;
	Subject_Attach(pThis->subject_, (IObserver_t *)pThis);	/* upcasting */
	printf("Hi, I'm the Observer \"%d\".\n", ++g_Observer_static_number_);
	pThis->number_ = g_Observer_static_number_;
	return pThis;
}

void Observer_RemoveMeFromTheList(Observer_t *pThis) {
	Subject_Detach(pThis->subject_, (IObserver_t *)pThis);	/* upcasting */
	printf("Observer \"%d\" removed from the list.\n", pThis->number_);
}


void ClientCode() {
  Subject_t subject;
  Subject_Init(&subject);
  
  Observer_t observer1;
  Observer_Init(&observer1, &subject);
  Observer_t observer2;
  Observer_Init(&observer2, &subject);
  Observer_t observer3;
  Observer_Init(&observer3, &subject);
  Observer_t observer4;
  Observer_t observer5;

  Subject_CreateMessage(&subject, "Hello World! :D");
  Observer_RemoveMeFromTheList(&observer3);

  Subject_CreateMessage(&subject, "The weather is hot today! :p");
  Observer_Init(&observer4, &subject);

  Observer_RemoveMeFromTheList(&observer2);
  Observer_Init(&observer5, &subject);

  Subject_CreateMessage(&subject, "My new car is great! ;)");
  Observer_RemoveMeFromTheList(&observer5);

  Observer_RemoveMeFromTheList(&observer4);
  Observer_RemoveMeFromTheList(&observer1);

  /* Called each destructor manually */  
  Observer_Destructor(&observer5);
  Observer_Destructor(&observer4);
  Observer_Destructor(&observer3);
  Observer_Destructor(&observer2);
  Observer_Destructor(&observer1);
  Subject_Destructor(&subject);
}

int main() {
  ClientCode();
  return 0;
}
