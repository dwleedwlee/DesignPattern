/* Source URL : https://github.com/Co-Le00/C-Memory-Pool */

#ifndef MEMORY_POOL
#define MEMORY_POOL

#ifndef API

	#ifdef __cplusplus
		
		#define EXTERN_C extern "C"
	
	#else

		#define EXTERN_C

	#endif
	
	#ifdef _WIN32

		#ifdef BUILD_MODE

			#define API __declspec(dllexport)

		#else

			#define API __declspec(dllimport)

		#endif

	#endif


#endif

#define dref(pointer) (*pointer)



/**

	Struct: memory_pool_t
	
	This structure holds essential parameters for allocating and deallocating memory blocks
	in memory pool. 

**/
typedef struct 
{
	/**

		Variable: number_of_blocks
		Holds number of total memory blocks in memory pool.
	
	**/
	unsigned int number_of_blocks;
	
	/**

		Variable: size_of_each_block
		Holds size of each memory block in memory pool.

	**/
	unsigned int size_of_each_block;

	/**
		
		Variable: number_of_free_blocks
		Holds number of total free blocks in memory pool.

	**/
	unsigned int number_of_free_blocks;

	/**

		Variable: number_of_initialized_blocks
		Holds number of total initialized blocks in memory pool.

	**/
	unsigned int number_of_init_blocks;

	/**
	
		Variable: memory_start_address
		Hold statring memory address of memory pool.

	**/
	unsigned char * memory_start_address;

	/**

		Variable: next
		Hold memory address of next memory for allocating.

	**/
	unsigned char * next;

} memory_pool_t;

/**

	Function: create_memory_pool
	Return type: void
	Params: * memory_pool_t, unsigned int, unisgned int

	This function creates new memory pool.

**/
EXTERN_C memory_pool_t * API create_memory_pool  (unsigned int type, unsigned int number_of_blocks);

/**

	Function: delete_memory_pool
	Return type: void
	Params: * memory_pool_t

	This function will delete given memory pool if it exists.

**/
EXTERN_C void   API delete_memory_pool  (memory_pool_t * memory_pool);

/**

	Function: allocate_memory_pool
	Return type: void *
	Params: * memory_pool_t

	This function will return next free memory address from created memory pool. If there
	are 0 free memory blocks it will return NULL.

**/
EXTERN_C void * API allocate_memory_pool (memory_pool_t * memory_pool);

/**
	
	Function: deallocate_memory_pool
	Return type: void 
	Params: memory_pool_t, void *

	This function will deallocate given memory block from memory pool if it exeists there.

**/
EXTERN_C void API deallocate_memory_pool (memory_pool_t * memory_pool, void * memory_address);

#endif