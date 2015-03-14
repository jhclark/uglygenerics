# uglygenerics
Automatically exported from code.google.com/p/uglygenerics

A (small) set of generically typed collection data structures (vector, hashmap) in pure C, currently in alpha. These have been implemented using the C preprocessor macros. Some people consider such things an ugly hack (hence the name), but I have found this to be the most type-safe way of doing business in C; this way, we avoid void pointers (which would all but reduce C to a weakly typed language). The macros have been set up to give the user maximum control over the behavior of the collection, providing user-definable options such as growth strategy, equality function, hash function, default return values, etc.
