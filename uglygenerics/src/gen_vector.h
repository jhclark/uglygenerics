#ifndef VECTOR_H_
#define VECTOR_H_

#include <gen_macros.h>

// TODO: Document that you should turn on range checking in GDB
// TODO: Onle define a vector for this name once (This must be handled by declaring header/implementations in their own files)
// TODO: recommended usage: declare each one in separate file

#define VECTOR_OMEM -1 	/* Out of Memory */
#define VECTOR_OK 0 	/* OK */

#define GEN_VECTOR_HEADER(T,NAME,GROWTH_FUNC) \
/* A vector has some maximum size and current size, \
* as well as the data to hold. \
* Access operations can be done directly on this structure \
*/ \
typedef struct CAT3(_,NAME,vecT) { \
	sizeT capacity; \
	sizeT size; \
	T* data; \
} CAT(NAME,vecT); \
/* \
 * Return an empty vector. Returns NULL if empty. \
 */ \
extern CAT(NAME,vecT)* CAT(NAME,vec_new) (sizeT init_sz); \
/* \
 * Add a string element to a vector \
 */ \
extern int CAT(NAME,vec_add) (CAT(NAME,vecT)* v, T item); \
/* \
 * Free a vector \
 */ \
extern void CAT(NAME,vec_free) (CAT(NAME,vecT)* v); \
/* \
 * Get an item from the vector \
 */ \
static inline T CAT(NAME,vec_get) (CAT(NAME,vecT)* v, sizeT i); \
\
/* \
 * Get the address of the last item in the vector \
 */ \
static inline T* CAT(NAME,vec_last) (CAT(NAME,vecT)* v);

/**
 * NOTE: Types are always suffixed with a single capital T
 * 
 * Required type definitions:
 * sizeT - the datatype representing the size of the vector
 * 
 * Recommended growth strategy:
 * vec_grow_quadratic
 */
#define GEN_VECTOR_IMPL(T,NAME,GROWTH_FUNC) \
/* \
 * Return an empty vector, or NULL on failure. \
 */ \
CAT(NAME,vecT)* CAT(NAME,vec_new) (sizeT init_sz) { \
	CAT(NAME,vecT)* m = (CAT(NAME,vecT)*) malloc(sizeof(CAT(NAME,vecT))); \
	if (m) { \
		m->data = (T*) calloc(init_sz, sizeof(T)); \
		if (m->data) { \
			m->capacity = init_sz; \
			m->size = 0; \
			return m; \
		} \
	} \
	if (m) \
		CAT(NAME,vec_free)(m); \
	return 0; \
} \
\
/* \
 * Doubles the size of the vector, and copies all the elements \
 */ \
int CAT(NAME,vec_grow)(CAT(NAME,vecT)* v) { \
	sizeT new_capacity = GROWTH_FUNC(v->capacity); \
	T* old_data = v->data; \
	T* new_data = (T*) calloc(new_capacity, sizeof(T)); \
	if (!new_data) \
		return VECTOR_OMEM; \
\
	T* old_ptr; \
	T* new_ptr; \
	T* last = CAT(NAME,vec_last)(v); \
	for(old_ptr=old_data, new_ptr=new_data; old_ptr<=last; old_ptr++, new_ptr++) \
		*new_ptr = *old_ptr; \
\
	v->data = new_data; \
	v->capacity = new_capacity; \
\
	free(old_data); \
\
	return VECTOR_OK; \
} \
\
/* \
 * Add an item to the vector \
 */ \
int CAT(NAME,vec_add) (CAT(NAME,vecT)* v, T item) { \
\
	/* grow vector if needed */ \
	if (v->size == v->capacity) { \
		if (CAT(NAME,vec_grow) (v) == VECTOR_OMEM) { \
			return VECTOR_OMEM; \
		} \
	} \
\
	/* Add the entry */ \
	v->data[v->size] = item; \
	v->size++; \
\
	return VECTOR_OK; \
} \
\
/* \
 * Get an item from the vector \
 */ \
static inline T CAT(NAME,vec_get) (CAT(NAME,vecT)* v, sizeT i) { \
	assert(i < v->size); \
	return v->data[i]; \
} \
\
/* \
 * Get the address of the last item in the vector \
 */ \
static inline T* CAT(NAME,vec_last) (CAT(NAME,vecT)* v) { \
	/* \
	 * NOTE: adding a value to a pointer multiplies \
	 * the non-pointer value by sizeof(pointer_type) \
	 */ \
	return v->data + (v->size - 1); \
} \
\
/* Deallocate the hashmap */ \
void CAT(NAME,vec_free) (CAT(NAME,vecT)* v) { \
	free(v->data); \
	free(v); \
}

#endif /*VECTOR_H_*/
