#ifndef GEN_GROW_H_
#define GEN_GROW_H_

static inline sizeT gen_grow_quadratic(sizeT cur_sz) {
	return cur_sz + cur_sz;
}

#endif /*GEN_GROW_H_*/
