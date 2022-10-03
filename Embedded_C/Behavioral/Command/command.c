
/* deferred or remote execution of commands, commands with state  */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct Command Command_t;

struct Command {
	void (*Execute)(Command_t *pThis);
};

typedef struct SimpleCommand {
	Command_t cmd;
	
	uint8_t *pay_load_;
}SimpleCommand_t;

void SimpleCommand_Execute(SimpleCommand_t *pThis) {
	printf("SimpleCommand: See, I can do simple things like printing (%s)\n", pThis->pay_load_);
}

SimpleCommand_t *SimpleCommand_Init(SimpleCommand_t *pThis, uint8_t *pay_load) {
	pThis->cmd.Execute = (void (*)(Command_t *))SimpleCommand_Execute;
	
	pThis->pay_load_ = pay_load;	//shallow copy
}

typedef struct Receiver {
	uint8_t anonymous;	/* empty variable */
} Receiver_t;

void Receiver_DoSomething(Receiver_t *pThis, const uint8_t *a) {
	printf("Receiver: Working on (%s.)\n", a);
}

void Receiver_DoSomethingElse(Receiver_t *pThis, const uint8_t *b) {
	printf("Receiver: Also working on (%s.)\n", b);
}

Receiver_t *Receiver_Init(Receiver_t *pThis) {
	return pThis;
}

typedef struct ComplexCommand {
	Command_t cmd;
	
	Receiver_t *receiver_;
	uint8_t *a_;
	uint8_t *b_;
} ComplexCommand_t;

void ComplexCommand_Execute(ComplexCommand_t *pThis) {
	printf("ComplexCommand: Complex stuff should be done by a receiver object.\n");
	Receiver_DoSomething(pThis->receiver_, pThis->a_);
	Receiver_DoSomethingElse(pThis->receiver_, pThis->b_);
}

ComplexCommand_t *ComplexCommand_Init(ComplexCommand_t *pThis, Receiver_t *receiver, uint8_t *a, uint8_t *b) {
	pThis->cmd.Execute = (void (*)(Command_t *))ComplexCommand_Execute;
	
	pThis->receiver_ = receiver;
	pThis->a_ = a; //shallow copy
	pThis->b_ = b; //shallow copy
}

typedef struct Invoker {
	Command_t *on_start_;
	
	Command_t *on_finish_;
} Invoker_t;

void Invoker_SetOnStart(Invoker_t *pThis, Command_t *command) {
	pThis->on_start_ = command;
}

void Invoker_SetOnFinish(Invoker_t *pThis, Command_t *command) {
	pThis->on_finish_ = command;
}	

void DoSomethingImportant(Invoker_t *pThis)	{
    printf("Invoker: Does anybody want something done before I begin?\n");
    if (pThis->on_start_) {
      pThis->on_start_->Execute(pThis->on_start_);
    }
    printf("Invoker: ...doing something really important...\n");
    printf("Invoker: Does anybody want something done after I finish?\n");
    if (pThis->on_finish_) {
      pThis->on_finish_->Execute(pThis->on_finish_);
    }
}

Invoker_t *Invoker_Init(Invoker_t *pThis) {
	return pThis;
}

int main() {
  Invoker_t invoker;
  Invoker_Init(&invoker);
  SimpleCommand_t simCmd;
  Invoker_SetOnStart(&invoker, (Command_t *)SimpleCommand_Init(&simCmd, "Say Hi!"));
  Receiver_t receiver;
  Receiver_Init(&receiver);
  ComplexCommand_t comCmd;
  Invoker_SetOnFinish(&invoker, (Command_t *)ComplexCommand_Init(&comCmd, &receiver, "Send email", "Save report"));
  DoSomethingImportant(&invoker);

  return 0;
}	
	