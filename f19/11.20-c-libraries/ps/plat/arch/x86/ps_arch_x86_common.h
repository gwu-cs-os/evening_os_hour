/***
 * Copyright 2015 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2015
 */

/*
 * TODO: most of this file should simply use the concurrency kit
 * versions.
 */

#ifndef PS_ARCH_X86_COMMON_H
#define PS_ARCH_X86_COMMON_H

typedef unsigned short int u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
typedef u64_t ps_tsc_t; 	/* our time-stamp counter representation */
typedef u16_t coreid_t;
typedef u16_t localityid_t;

#ifndef likely
#define likely(x)      __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif

#define PS_CACHE_LINE       64
#define PS_CACHE_PAD        (PS_CACHE_LINE*2)
#define PS_CACHE_PAD_SZ(sz) (PS_CACHE_PAD - ((sz) % PS_CACHE_PAD))
#define PS_WORD             sizeof(long)
#define PS_PACKED           __attribute__((packed))
#define PS_ALIGNED          __attribute__((aligned(PS_CACHE_LINE)))
#define PS_WORDALIGNED      __attribute__((aligned(PS_WORD)))
#define PS_PAGE_SIZE        4096
#define PS_RNDUP(v, a)      (-(-(v) & -(a))) /* from blogs.oracle.com/jwadams/entry/macros_and_powers_of_two */

#define PS_CAS_INSTRUCTION "cmpxchg"
#define PS_FAA_INSTRUCTION "xadd"
#define PS_CAS_STR PS_CAS_INSTRUCTION PS_ATOMIC_POSTFIX " %2, %0; setz %1"
#define PS_FAA_STR PS_FAA_INSTRUCTION PS_ATOMIC_POSTFIX " %1, %0"

#ifndef ps_cc_barrier
#define ps_cc_barrier() __asm__ __volatile__ ("" : : : "memory")
#endif

/*
 * Return values:
 * 0 on failure due to contention (*target != old)
 * 1 otherwise (*target == old -> *target = updated)
 */
static inline int
ps_cas(unsigned long *target, unsigned long old, unsigned long updated)
{
        char z;
        __asm__ __volatile__("lock " PS_CAS_STR
                             : "+m" (*target), "=a" (z)
                             : "q"  (updated), "a"  (old)
                             : "memory", "cc");
        return (int)z;
}

static inline long
ps_faa(unsigned long *target, long inc)
{
        __asm__ __volatile__("lock " PS_FAA_STR
                             : "+m" (*target), "+q" (inc)
                             : : "memory", "cc");
        return inc;
}

static inline void
ps_mem_fence(void)
{ __asm__ __volatile__("mfence" ::: "memory"); }

#define ps_load(addr) (*(volatile __typeof__(*addr) *)(addr))
#define ps_store(addr, val) ((*(volatile __typeof__(*addr) *)(addr)) = val)

/*
 * Only atomic on a uni-processor, so not for cross-core coordination.
 * Faster on a multiprocessor when used to synchronize between threads
 * on a single core by avoiding locking.
 */
static inline int
ps_upcas(unsigned long *target, unsigned long old, unsigned long updated)
{
        char z;
        __asm__ __volatile__(PS_CAS_STR
                             : "+m" (*target), "=a" (z)
                             : "q"  (updated), "a"  (old)
                             : "memory", "cc");
        return (int)z;
}

static inline long
ps_upfaa(unsigned long *target, long inc)
{
        __asm__ __volatile__(PS_FAA_STR
                             : "+m" (*target), "+q" (inc)
                             : : "memory", "cc");
        return inc;
}


/*
 * FIXME: this is truly an affront to humanity for now, but it is a
 * simple lock for testing -- naive spin *without* backoff, gulp
 *
 * This is a great example where we should be using CK.
 */
struct ps_lock {
	unsigned long o;
};

static inline void
ps_lock_take(struct ps_lock *l)
{ while (!ps_cas(&l->o, 0, 1)) ; }

static inline void
ps_lock_release(struct ps_lock *l)
{ l->o = 0; }

static inline void
ps_lock_init(struct ps_lock *l)
{ l->o = 0; }

static inline ps_tsc_t
ps_tsc(void)
{
	unsigned long a, d, c;

	__asm__ __volatile__("rdtsc" : "=a" (a), "=d" (d), "=c" (c) : : );

	return ((u64_t)d << 32) | (u64_t)a;
}

#endif /* PS_ARCH_X86_COMMON_H */
