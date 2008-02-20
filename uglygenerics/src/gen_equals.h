#ifndef GEN_EQUALS_H_
#define GEN_EQUALS_H_

#include <string.h>

static inline boolT gen_int_equals(int a, int b) {
	return a == b;
}

static inline boolT gen_str_equals(char* a, char* b) {
	return strcmp(a,b) == 0;
}

#endif /*GEN_EQUALS_H_*/
