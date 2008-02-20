#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <gen_macros.h>

/**
 * Required typedefs:
 * hashT - data type used for storing hash values
 * 			(recommend using uint32_t from stdint.h)
 * boolT - boolean data type
 * 
 * Macro arguments:
 * T - Datatype of the key
 * V - Datatype of the value
 * NAME - Unique identifier for this type of hashmap
 * DEFAULT_RETURN_V - A value to be returned if the specified key was
 * 						not found in the map
 * GROWTH_FUNC - Function that takes the current size of the hashmap
 * 					and returns a new, larger size
 * HASH_FUNC - Function that maps a key to a hashT
 * EQUALS_FUNC - Function that compares 2 keys for equality
 * 					and returns 1 iff they are the same
 * PROBING_FUNC - Function that determines how probing is performed
 * 					when collisions are encountered.
 */

// TODO: Custom probing function

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

#define GEN_HASHMAP_HEADER(T,V,NAME,DEFAULT_RETURN_V,GROWTH_FUNC,HASH_FUNC,EQUALS_FUNC) \
\
/* We need to keep keys and values */ \
typedef struct CAT3(_,NAME,hashmap_eT) { \
	T key; \
	boolT in_use; \
	V data; \
} CAT(NAME,hashmap_eT); \
\
/* \
* A hashmap has some maximum size and current size, \
* as well as the data to hold. \
*/ \
typedef struct CAT3(_,NAME,hashmapT) { \
	sizeT capacity; \
	sizeT size; \
	float load_factor; \
	sizeT threshold; \
	CAT(NAME,hashmap_eT)* data; \
} CAT(NAME,hashmapT); \
\
/* \
 * PFany is a pointer to a function that can take two any_t arguments \
 * and return an integer. Returns status code.. \
 */ \
typedef int (*CAT(NAME,iterator_funcPT))(T, V); \
/* \
 * Return an empty hashmap. Returns NULL if empty. \
 */ \
extern CAT(NAME,hashmapT)* CAT(NAME,hashmap_new)(sizeT init_sz, float load_factor); \
\
/* \
 * Iteratively call f with argument (item, data) for \
 * each element data in the hashmap. The function must \
 * return a map status code. If it returns anything other \
 * than MAP_OK the traversal is terminated. f must \
 * not reenter any hashmap functions, or deadlock may arise. \
 */ \
extern int CAT(NAME,hashmap_iterate)(CAT(NAME,hashmapT)* m, CAT(NAME,iterator_funcPT) f); \
\
/* \
 * Add an element to the hashmap. Return MAP_OK or MAP_OMEM. \
 */ \
extern int CAT(NAME,hashmap_put)(CAT(NAME,hashmapT)* m, T key, V value); \
\
/* \
 * Get an element from the hashmap. Return MAP_OK or MAP_MISSING. \
 */ \
extern V CAT(NAME,hashmap_get)(CAT(NAME,hashmapT)* m, T key); \
\
/* \
 * Remove an element from the hashmap. Return MAP_OK or MAP_MISSING. \
 */ \
extern int CAT(NAME,hashmap_remove)(CAT(NAME,hashmapT)* m, T key); \
\
/* \
 * Free the hashmap \
 */ \
extern void CAT(NAME,hashmap_free)(CAT(NAME,hashmapT)* m); \
\
/* \
 * Get the current size of a hashmap \
 */ \
extern int CAT(NAME,hashmap_size)(CAT(NAME,hashmapT)* m);

#define GEN_HASHMAP_IMPL(T,V,NAME,DEFAULT_RETURN_V,GROWTH_FUNC,HASH_FUNC,EQUALS_FUNC) \
\
/* \
 * Return an empty hashmap, or NULL on failure. \
 */ \
CAT(NAME,hashmapT)* CAT(NAME,hashmap_new)(sizeT init_sz, float load_factor) { \
	CAT(NAME,hashmapT)* m = (CAT(NAME,hashmapT)*) malloc(sizeof(CAT(NAME,hashmapT))); \
	if (m) { \
		m->data = (CAT(NAME,hashmap_eT)*) calloc(init_sz, sizeof(CAT(NAME,hashmap_eT))); \
		if (m->data) { \
/*			int i; \
			for(i=0; i<init_sz; i++) \
				m->data[i] = 0; */ \
			m->capacity = init_sz; \
			m->size = 0; \
			m->load_factor = load_factor; \
			m->threshold = (sizeT) (init_sz * load_factor); \
			return m; \
		} \
	} \
	if (m) \
		CAT(NAME,hashmap_free)(m); \
	return 0; \
} \
\
/* \
 * Return the integer of the location in data \
 * to store the point to the item, or MAP_FULL. \
 */ \
sizeT CAT(NAME,hashmap_hash)(CAT(NAME,hashmapT)* m, T key) { \
	sizeT curr; \
	sizeT i; \
\
	/* If full, return immediately */ \
	if (m->size >= m->threshold) \
		return MAP_FULL; \
\
	/* Find the best index */ \
	curr = HASH_FUNC(key) % m->capacity; \
\
	/* Linear probling */ \
	for (i = 0; i< m->capacity; i++) { \
		if (m->data[curr].in_use == 0) \
			return curr; \
\
		if (m->data[curr].in_use == 1 && EQUALS_FUNC(m->data[curr].key, key)) \
			return curr; \
\
		curr = (curr + 1) % m->capacity; \
	} \
\
	return MAP_FULL; \
} \
\
/* \
 * Doubles the size of the hashmap, and rehashes all the elements \
 */ \
int CAT(NAME,hashmap_rehash)(CAT(NAME,hashmapT)* m) { \
	sizeT i; \
	sizeT old_size; \
	sizeT new_size; \
	CAT(NAME,hashmap_eT)* curr; \
\
	old_size = m->capacity; \
	new_size = GROWTH_FUNC(old_size); \
	CAT(NAME,hashmap_eT)* temp = (CAT(NAME,hashmap_eT)*)calloc(new_size, \
			sizeof(CAT(NAME,hashmap_eT))); \
	if (!temp) \
		return MAP_OMEM; \
/* \
	for(i=0; i<new_size; i++) \
		temp[i] = 0; */ \
\
	/* Update the array */ \
	curr = m->data; \
	m->data = temp; \
\
	/* Update the size */ \
	m->capacity = new_size; \
	m->size = 0; \
	m->threshold = (sizeT) (new_size * m->load_factor); \
\
	/* Rehash the elements */ \
	for (i=0; i<old_size; i++) { \
		if(curr[i].in_use) { \
			int status = CAT(NAME,hashmap_put)(m, curr[i].key, curr[i].data); \
			if (status != MAP_OK) \
				return status; \
		} \
	} \
\
	free(curr); \
\
	return MAP_OK; \
} \
\
/* \
 * Add a pointer to the hashmap with some key \
 */ \
int CAT(NAME,hashmap_put)(CAT(NAME,hashmapT)* m, T key, V value) { \
	sizeT index; \
\
	/* Find a place to put our value */ \
	index = CAT(NAME,hashmap_hash)(m, key); \
	while (index == MAP_FULL) { \
		if (CAT(NAME,hashmap_rehash)(m) == MAP_OMEM) { \
			return MAP_OMEM; \
		} \
		index = CAT(NAME,hashmap_hash)(m, key); \
	} \
\
	/* Set the data */ \
	m->data[index].data = value; \
	m->data[index].key = key; \
	m->data[index].in_use = 1; \
	m->size++; \
\
	return MAP_OK; \
} \
\
/* \
 * Get your pointer out of the hashmap with a key \
 */ \
V CAT(NAME,hashmap_get)(CAT(NAME,hashmapT)* m, T key) { \
	sizeT curr; \
	sizeT i; \
\
	/* Find data location */ \
	curr = HASH_FUNC(key) % m->capacity; \
\
	/* Linear probing, if necessary */ \
	for (i = 0; i<m->capacity; i++) { \
		if (m->data[curr].in_use == 1 && EQUALS_FUNC(m->data[curr].key, key) == 1) { \
			return m->data[curr].data; \
		} \
		curr = (curr + 1) % m->capacity; \
	} \
	return DEFAULT_RETURN_V; \
} \
\
/* \
 * Iterate the function parameter over each element in the hashmap.  The \
 * additional any_t argument is passed to the function as its first \
 * argument and the hashmap element is the second. \ 
 */ \
int CAT(NAME,hashmap_iterate)(CAT(NAME,hashmapT)* m, CAT(NAME,iterator_funcPT) f) { \
	sizeT i; \
\
	/* On empty hashmap, return immediately */ \
	if (CAT(NAME,hashmap_size)(m) <= 0) \
		return MAP_MISSING; \
\
	/* Linear probing */ \
	for (i = 0; i< m->capacity; i++) \
		if (m->data[i].in_use != 0) { \
			T key = m->data[i].key; \
			V data = m->data[i].data; \
			int status = f(key, data); \
			if (status != MAP_OK) { \
				return status; \
			} \
		} \
	return MAP_OK; \
} \
\
/* \
 * Remove an element with that key from the map \
 */ \
int CAT(NAME,hashmap_remove)(CAT(NAME,hashmapT)* m, T key) { \
	sizeT i; \
	sizeT curr; \
\
	/* Find key */ \
	curr = HASH_FUNC(key) % m->capacity; \
\
	/* Linear probing, if necessary */ \
	for (i = 0; i< m->capacity; i++) { \
		if (m->data[curr].key == key && m->data[curr].in_use == 1) { \
			/* Blank out the fields */ \
			m->data[curr].in_use = 0; \
			m->data[curr].data = 0; \
			m->data[curr].key = 0; \
\
			/* Reduce the size */ \
			m->size--; \
			return MAP_OK; \
		} \
		curr = (curr + 1) % m->capacity; \
	} \
\
	/* Data not found */ \
	return MAP_MISSING; \
} \
\
/* Deallocate the hashmap */ \
void CAT(NAME,hashmap_free)(CAT(NAME,hashmapT)* m) { \
	free(m->data); \
	free(m); \
} \
\
/* Return the length of the hashmap */ \
int CAT(NAME,hashmap_size)(CAT(NAME,hashmapT)* m) { \
	if (m != 0) \
		return m->size; \
	else \
		return 0; \
}

#endif HASHMAP_H_
