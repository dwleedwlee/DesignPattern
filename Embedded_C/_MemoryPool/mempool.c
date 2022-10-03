/* Source URL : https://github.com/Co-Le00/C-Memory-Pool */

#include "mempool.h"

#include <stdlib.h>
#include <stdio.h>

static unsigned int index_from_addr(memory_pool_t * memory_pool, const unsigned char * addr)
{
	return ((unsigned int) (addr - dref(memory_pool).memory_start_address)) / (dref(memory_pool).size_of_each_block);
}

static unsigned char * addr_from_index(memory_pool_t * memory_pool, unsigned int index)
{
	return dref(memory_pool).memory_start_address + (index * dref(memory_pool).size_of_each_block);
}

EXTERN_C memory_pool_t * API create_memory_pool  (unsigned int type, unsigned int number_of_blocks)
{

	#if DEBUG_MODE
		printf("[+] Calling: create_memory_pool\n");
	#endif

	// Create new memory pool
	memory_pool_t * memory_pool = (memory_pool_t *) malloc(sizeof(memory_pool_t));

	// Initialize its values
	dref(memory_pool).number_of_blocks 		= number_of_blocks;
	dref(memory_pool).size_of_each_block 	= type;
	dref(memory_pool).number_of_free_blocks = number_of_blocks;
	dref(memory_pool).number_of_init_blocks = 0;
	dref(memory_pool).memory_start_address  = (unsigned char *) malloc(number_of_blocks * type);
	dref(memory_pool).next = dref(memory_pool).memory_start_address;

	#if DEBUG_MODE
		printf("[+] Ending:  create_memory_pool\n");
	#endif

	return memory_pool;
}

EXTERN_C void   API delete_memory_pool  (memory_pool_t * memory_pool)
{
	#if DEBUG_MODE
		printf("[+] Calling: delete_memory_pool\n");
	#endif

	// First free memory pool starting address
	free(dref(memory_pool).memory_start_address);

	#if DEBUG_MODE
		printf("[+] Memory pool starting address: %x\n", dref(memory_pool).memory_start_address);
	#endif

	// Then free memory pool struct
	free(memory_pool);

	#if DEBUG_MODE
		printf("[+] Memory pool struct address:   %x\n", memory_pool);
	#endif

	#if DEBUG_MODE
		printf("[+] Ending:  delete_memory_pool\n");
	#endif
}

EXTERN_C void * API allocate_memory_pool(memory_pool_t * memory_pool)
{
	if(dref(memory_pool).number_of_free_blocks > 0)
	{
		unsigned int * helper = (unsigned int *) addr_from_index(memory_pool, dref(memory_pool).number_of_init_blocks);
		*helper = dref(memory_pool).number_of_init_blocks + 1;
		dref(memory_pool).number_of_init_blocks += 1;
	}
	void * ret = NULL;

	if(dref(memory_pool).number_of_init_blocks > 0)
	{
		ret = (void *) dref(memory_pool).next;
		dref(memory_pool).number_of_free_blocks -= 1;

		if(dref(memory_pool).number_of_free_blocks != 0)
		{
			dref(memory_pool).next = addr_from_index(memory_pool, *((unsigned int*) dref(memory_pool).next));
		}
		else
		{
			dref(memory_pool).next = NULL;
		}
	}
	return ret;
}

EXTERN_C void API deallocate_memory_pool (memory_pool_t * memory_pool, void * memory_address)
{
	if(dref(memory_pool).next != NULL)
	{
		*(unsigned int *) memory_address = index_from_addr(memory_pool, dref(memory_pool).next);
		dref(memory_pool).next = (unsigned char *) memory_address;
	}
	else
	{
		*(unsigned int *) memory_address = dref(memory_pool).number_of_init_blocks;
		dref(memory_pool).next = (unsigned char *) memory_address;
	}
	dref(memory_pool).number_of_free_blocks += 1;
	memory_address = NULL;
}