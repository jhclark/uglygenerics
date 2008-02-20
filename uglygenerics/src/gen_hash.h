#ifndef GEN_HASH_H_
#define GEN_HASH_H_

/*
 * Hashing function for an integer
 */
static inline hashT gen_hash_int(unsigned int key) {
	/* Robert Jenkins’ 32 bit Mix Function */
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

	/* Knuth’s Multiplicative Method */
	key = (key >> 3) * 2654435761UL;
	return key;
}

/**
 * Hashing function for a string
 */
static inline hashT gen_hash_str(char* str) {
	hashT hash = 5381;
	hashT c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

#endif /*GEN_HASH_H_*/
