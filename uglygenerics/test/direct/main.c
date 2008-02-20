#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <typedefs.h>
#include <gen_hash.h>
#include <gen_grow.h>
#include <gen_equals.h>

#include <gen_vector.h>
#include <gen_hashmap.h>

// #define GEN_VECTOR_HEADER(T,NAME,GROWTH_FUNC) 
GEN_VECTOR_HEADER(char*,str,gen_grow_quadratic)
GEN_VECTOR_IMPL(char*,str,gen_grow_quadratic)

// #define GEN_HASHMAP_HEADER(T,V,NAME,GROWTH_FUNC,HASH_FUNC,EQUALS_FUNC)
GEN_HASHMAP_HEADER(char*,int,str2int,-1,gen_grow_quadratic,gen_hash_str,gen_str_equals)
GEN_HASHMAP_IMPL(char*,int,str2int,-1,gen_grow_quadratic,gen_hash_str,gen_str_equals)

int main(int argc, char** argv) {
	
	printf("VECTOR TEST:\n");
	str_vecT* vec = str_vec_new(1);
	
	str_vec_add(vec, "hi");
	str_vec_add(vec, "bye");
	str_vec_add(vec, "1");
	str_vec_add(vec, "2");
	str_vec_add(vec, "3");
	
	int i;
	for(i=0;i<vec->size;i++)
		printf("%s\n", str_vec_get(vec,i));
	
	printf("--------\n");
	
	char** ptr;
	char** last = str_vec_last(vec);
	for(ptr=vec->data; ptr<=last; ptr++)
		printf("%p: %s\n", ptr, *ptr);
	
	str_vec_free(vec);
	
	printf("HASHAP TEST:\n");
	
	str2int_hashmapT* hash = str2int_hashmap_new(1, 0.75);
	
	str2int_hashmap_put(hash, "one", 1);
	str2int_hashmap_put(hash, "two", 2);
	str2int_hashmap_put(hash, "three", 3);
	str2int_hashmap_put(hash, "four", 4);
	str2int_hashmap_put(hash, "five", 5);
	
	printf("%d\n", str2int_hashmap_get(hash, "one"));
	printf("%d\n", str2int_hashmap_get(hash, "two"));
	printf("%d\n", str2int_hashmap_get(hash, "three"));
	printf("%d\n", str2int_hashmap_get(hash, "four"));
	printf("%d\n", str2int_hashmap_get(hash, "five"));
	
	printf("%d\n", str2int_hashmap_get(hash, "one hundred"));
	
	str2int_hashmap_free(hash);
	
	return 0;
}