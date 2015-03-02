
typedef struct { int counter; } atomic_t;
#define atomic_set(v, i)	((v)->counter = (i))

static inline void atomic_add(atomic_t *v, int incr)
{
	asm volatile (
		"lock xaddl %1, %0\n" 
		: "+m" (v->counter)
		: "ir" (incr)
	);
}

static inline void atomic_sub(atomic_t *v, int incr)
{
	asm volatile (
		"lock subl %1, %0\n" 
		: "+m" (v->counter)
		: "ir" (incr)
	);
}

static inline int atomic_read(const atomic_t *v)
{
	return (*(volatile int *)&(v)->counter);
}

