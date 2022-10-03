
/* object tree, A superset(composite) is also a subset(component) of a system */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "linkedlist.h"

typedef struct Component Component_t;

struct Component {
	Component_t *parent_;
	
	void (*Add)(Component_t *pThis, Component_t *component);
    void (*Remove)(Component_t *pThis, Component_t *component);
    bool (*IsComposite)(Component_t *pThis);
    uint8_t *(*Operation)(Component_t *pThis);
};

void Component_SetParent(Component_t *pThis, Component_t *parent) {
	pThis->parent_ = parent;
}

Component_t *Component_GetParent(Component_t *pThis) {
	return pThis->parent_;
}

void Component_Add(Component_t *pThis, Component_t *component) {
	/* empty function */
}

void Component_Remove(Component_t *pThis, Component_t *component) {
	/* empty function */
}

bool Component_IsComposite(Component_t *pThis) {
	return false;
}

Component_t *Component_Init(Component_t *pThis) {
	pThis->Add = Component_Add;
	pThis->Remove = Component_Remove;
	pThis->IsComposite = Component_IsComposite;
	return pThis;
}

typedef struct Leaf {
	Component_t comp;
} Leaf_t;

uint8_t *Leaf_Operation(Leaf_t *pThis) {
	return "Leaf";
}

Leaf_t *Leaf_Init(Leaf_t *pThis) {
	pThis->comp.Operation = (uint8_t *(*)(Component_t *))Leaf_Operation;
	return pThis;
}

typedef struct Composite {
	Component_t comp;
	Node *children_;
}Composite_t;	
	
void Composite_Add(Composite_t *pThis, Component_t *component) {
	pThis->children_ = postInsert(pThis->children_, (int)component); /* Component_t * -> int */
	Component_SetParent(component, (Component_t *)pThis); /* upcasting */
}

void Composite_Remove(Composite_t *pThis, Component_t *component) {
	pThis->children_ = deleteNode(pThis->children_, (int)component);  /* Component_t * -> int */
	Component_SetParent(component, NULL);
}

bool Composite_IsComposite(Composite_t *pThis) {
	return true;
}

uint8_t *Composite_Operation(Composite_t *pThis) {
	uint8_t *result;
	
	/* FIXME : allocated fixed size stack memory */
	uint8_t buf[256] = "";	/* str instance */
	/* FIXME : neet to allocate the buffer size statically */
	static uint8_t ret_buf[256] = "";

	Node* nd = pThis->children_;
	uint32_t numNodes = countNodes(nd);
	uint32_t count = 0;
	uint32_t offset = 0;
	while (nd != NULL) {
		Component_t *comp = (Component_t *)(nd->n);	/* int -> Component_t * */
		uint8_t *str = comp->Operation(comp);

		if(++count >= numNodes) {
			sprintf(buf + offset, "%s", str);
			offset += strlen(str);
		} else {
			sprintf(buf + offset, "%s%s", str, "+");
			offset += (strlen(str) + 1);
		}
		nd = nd->next;
	}
	sprintf(ret_buf, "%s%s%s", "Branch(", buf, ")");
	return ret_buf;
}

Composite_t *Composite_Init(Composite_t *pThis) {
	pThis->children_ = NULL;
	pThis->comp.Add = (void (*)(Component_t *, Component_t *))Composite_Add;
	pThis->comp.Remove = (void (*)(Component_t *, Component_t *))Composite_Remove;
	pThis->comp.IsComposite = (bool (*)(Component_t *))Composite_IsComposite;
	pThis->comp.Operation = (uint8_t *(*)(Component_t *))Composite_Operation;
	return pThis;
}

void ClientCode(Component_t *component) {
  // ...
  printf("RESULT: %s", component->Operation(component));
  // ...
}

void ClientCode2(Component_t *component1, Component_t *component2) {
  // ...
  if (component1->IsComposite(component1)) {
    component1->Add(component1, component2);
  }
  printf("RESULT: %s", component1->Operation(component1));
  // ...
}

int main() {
  Leaf_t leaf;	
  Leaf_Init(&leaf);
  Component_t *simple = (Component_t *)&leaf;
  
  printf("Client: I've got a simple component:\n");
  ClientCode(simple);
  printf("\n\n");
  
  /**
   * ...as well as the complex composites.
   */
  Composite_t comp1, comp2;
  Composite_Init(&comp1);
  Composite_Init(&comp2);
  Component_t *tree = (Component_t *)&comp1;

  Component_t *branch1 = (Component_t *)&comp2;
  Leaf_t leaf1, leaf2, leaf3;
  Leaf_Init(&leaf1);
  Leaf_Init(&leaf2);
  Leaf_Init(&leaf3);  
  Component_t *leaf_1 = (Component_t *)&leaf1;
  Component_t *leaf_2 = (Component_t *)&leaf2;
  Component_t *leaf_3 = (Component_t *)&leaf3;  
  
  branch1->Add(branch1, leaf_1);
  branch1->Add(branch1, leaf_2);
  
  Composite_t comp3;
  Composite_Init(&comp3);
  Component_t *branch2 = (Component_t *)&comp3;
  branch2->Add(branch2, leaf_3);
  
  tree->Add(tree, branch1);
  tree->Add(tree, branch2);
  printf("Client: Now I've got a composite tree:\n");
  ClientCode(tree);
  printf("\n\n");

  printf("Client: I don't need to check the components classes even when managing the tree:\n");
  ClientCode2(tree, simple);
  printf("\n");

  return 0;
}
